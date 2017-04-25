#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../include/server/Server.h"
#include "../../include/helpers/utils.h"
#include "../../include/http/http.h"

#define CHUNK 256

Server::Server(int port, const std::string &rootDir) {
    this->port = port;
    socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket <= 0) {
        cleanUp();
        throw std::runtime_error("Unable to create socket: " + std::string(strerror(errno)));
    }

    int options = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options));

    /*int currentFlags = fcntl(socket, F_GETFL, 0);
    int setFlags = currentFlags | O_NONBLOCK;
    if (fcntl(socket, F_SETFL, setFlags) == -1) {
        cleanUp();
        throw std::runtime_error("Unable to make socket non-blocking: " + std::string(strerror(errno)));
    } */

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int binded = bind(socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (binded < 0) {
        cleanUp();
        close(socket);
        throw std::runtime_error("Unable to bind socket: " + std::string(strerror(errno)));
    }

    this->rootDir = rootDir;
    worker = new WorkerThread(rootDir);
    listen(socket, DEFAULT_BACKLOG_SIZE);
}

Server::~Server() {
    utils::log("~Server()");
    cleanUp();
    if (socket > 0) {
        close(socket);
    }
}

void Server::cleanUp() {
    delete worker;
}

int Server::getPort() { return port; }

std::string Server::getRootDir() {
    return rootDir;
}

int Server::start() {
    if (socket <= 0) {
        throw new std::runtime_error("Socket was not handled");
    }
    if (isWorking) {
        return 0;
    }
    if (!worker->launch()) {
        return 0;
    }
    isWorking = true;

    while (isWorking) {
        struct sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(socket, (struct sockaddr*)&clientAddr, &clientLen);
        ClientHandler *client = new ClientHandler(clientSocket);
        worker->handleClient(&client);
    }

    return 0;
}

void Server::stop() {
    if (socket <= 0) {
        throw new std::runtime_error("Socket was not handled");
    }
    if (isWorking) {
        isWorking = false;
    }
}