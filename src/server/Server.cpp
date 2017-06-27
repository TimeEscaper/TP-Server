#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "../../include/server/Server.h"
#include "../../include/http/http.h"
#include "../../include/server/ClientHandler.h"
#include "../../include/server/ClientHandleTask.h"
#include "../../include/logging/utils.h"

#define CHUNK 256

Server::Server(int port, const std::string &rootDir, int threadPoolSize, int ncpu) {
    this->port = port;
    socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket <= 0) {
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
        close(socket);
        throw std::runtime_error("Unable to bind socket: " + std::string(strerror(errno)));
    }

    this->rootDir = rootDir;
    threadPool = new ThreadPool(threadPoolSize, ncpu);

    listen(socket, DEFAULT_BACKLOG_SIZE);
}

Server::~Server() {
    log::log("~Server()");
    cleanUp();
    if (socket > 0) {
        close(socket);
    }
}

void Server::cleanUp() {
    if (threadPool != NULL) {
        delete threadPool;
    }
}

int Server::start() {
    if (socket <= 0) {
        throw new std::runtime_error("Socket was not handled");
    }
    if (isWorking) {
        return 0;
    }

    isWorking = true;

    while (isWorking) {
        struct sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(socket, (struct sockaddr*)&clientAddr, &clientLen);
        ClientHandler *client = new ClientHandler(clientSocket);

        ClientHandleTask *task = new ClientHandleTask(&client, rootDir);
        threadPool->pushTask(&task);
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