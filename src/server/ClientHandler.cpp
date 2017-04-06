#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include "../../include/server/ClientHandler.h"
#include "../../include/helpers/utils.h"
#include "../../include/server/Server.h"

ClientHandler::ClientHandler(int socket) {
    if (socket <= 0) {
        throw std::runtime_error("Unable to handle client: socket <= 0");
    }
    this->socket = socket;
}

ClientHandler::~ClientHandler() {
    if (socket > 0) {
        close(socket);
    }
}

long ClientHandler::sendRaw(const char* data) {
    size_t dataSize = strlen(data);
    size_t left = strlen(data);
    ssize_t sent = 0;
    ssize_t totalSent = 0;
    while (left > 0) {
        sent = ::send(socket, data + sent, dataSize - sent, 0);
        if (sent == -1) {
            utils::log("Unable to send data!");
            return totalSent;
        }
        left -= sent;
        totalSent += sent;
    }
    return totalSent;
}

//TODO: chunk buffer
void ClientHandler::receiveRaw(long* received, char** receivePtr) {
    *receivePtr = new char[DEFAULT_REQUEST_BUFFER];
    ssize_t read = recv(socket, *receivePtr, DEFAULT_REQUEST_BUFFER, 0); //Blocking
    if (read<0) {
        std::cout << read << std::endl;
    }
    (*receivePtr)[read] = '\0';
    *received = read + 1;
}