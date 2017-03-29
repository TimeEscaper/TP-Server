#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "../../include/server/Server.h"
#include "../../include/helpers/utils.h"

Server::Server(int port) {
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
    listen(socket, DEFAULT_BACKLOG_SIZE);

    utils::log("Server created!");
}

Server::~Server() {
    if (socket > 0) {
        close(socket);
    }
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
    std::cout << request << std::endl;
    client.sendRaw("HTTP/1.1 200 OK\r\n\r\n<html><body><b>Hello, World!</body></html>\r\n");
}