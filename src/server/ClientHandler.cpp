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

void ClientHandler::sendRaw(const char* data) {
    size_t dataSize = strlen(data);
    size_t left = strlen(data);
    ssize_t sent = 0;
    while (left > 0) {
        sent = ::send(socket, data + sent, dataSize - sent, 0);
        if (sent == -1) {
            utils::log("Unable to send data!");
            return;
        }
        left -= sent;
    }
}

char* ClientHandler::receiveRaw() {
    size_t bufferSize = DEFAULT_REQUEST_BUFFER;
    char* buffer = new char[bufferSize];
    size_t read = recv(socket, buffer, bufferSize, 0); //Blocking
    if (read<0) {
        std::cout << read << std::endl;
        delete[] buffer;
        return NULL;
    }

    return buffer;
}