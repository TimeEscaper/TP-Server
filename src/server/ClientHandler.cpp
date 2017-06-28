#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include "../../include/server/ClientHandler.h"
#include "../../include/server/Server.h"
#include "../../include/logging/log.h"

#define DEFAULT_REQUEST_BUFFER 1024

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

long ClientHandler::sendRaw(const std::string &data) {
    const char* dataPtr = data.c_str();
    ssize_t dataSize = data.length();
    ssize_t left = strlen(dataPtr);
    ssize_t sent = 0;
    ssize_t totalSent = 0;
    while (left > 0) {
        sent = ::send(socket, dataPtr + sent, dataSize - sent, 0);
        if (sent == -1) {
            log::debug("Unable to send data!");
            return totalSent;
        }
        left -= sent;
        totalSent += sent;
    }
    return totalSent;
}

long ClientHandler::sendRaw(const char *data, ssize_t dataLength) {
    ssize_t left = dataLength;
    ssize_t sent = 0;
    ssize_t totalSent = 0;
    while (left > 0) {
        sent = ::send(socket, data + sent, dataLength - sent, 0);
        if (sent == -1) {
            log::debug("Unable to send data!");
            return totalSent;
        }
        left -= sent;
        totalSent += sent;
    }
    return totalSent;
}

//TODO: chunk buffer
std::string ClientHandler::receiveRaw() {
    std::string result;
    char buffer[DEFAULT_REQUEST_BUFFER];
    ssize_t read = recv(socket, buffer, DEFAULT_REQUEST_BUFFER, 0); //Blocking
    if (read > 0) {
        result.append(buffer, read);
    }
    return result;
}