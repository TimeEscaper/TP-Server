#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "../../include/server/Server.h"
#include "../../include/helpers/utils.h"
#include "../../include/http/http.h"

Server::Server(int port, const char* rootDir) {
    this->port = port;
    socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket <= 0)
        return;

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int binded = bind(socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (binded < 0) {
        close(socket);
        return;
    }

    strcpy(this->rootDir, rootDir);
    listen(socket, DEFAULT_BACKLOG_SIZE);

    utils::log("Server created!");
    utils::log(rootDir);
}

Server::~Server() {
    if (socket > 0) {
        close(socket);
    }
}

int Server::getPort() { return port; }

char* Server::getRootDir() {
    char* res = new char;
    strcpy(res, rootDir);
    return res;
}

void Server::start() {
    if ((isWorking) || (socket <= 0)) {
        return;
    }
    isWorking = true;

    while (isWorking) {
        struct sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(socket, (struct sockaddr*)&clientAddr, &clientLen);
        ClientHandler client(clientSocket);
        handleClient(client);
    }
}

void Server::handleClient(ClientHandler client) {
    char* request = client.receiveRaw();
    utils::log(request);
    client.sendRaw(HTTP200RAW);
}