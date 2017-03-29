#include <unistd.h>
#include <sys/socket.h>
#include "../../include/server/ClientHandler.h"
#include "../../include/helpers/utils.h"

ClientHandler::ClientHandler(int socket) {
    this->socket = socket; //TODO: check on < 0
}

ClientHandler::~ClientHandler() {
    if (socket > 0) {
        close(socket);
    }
}

void ClientHandler::sendRaw(const char* data) {
    size_t dataSize = sizeof(data);
    size_t left = sizeof(data);
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
    size_t bufferSize = DEFAULT_REQUEST_BUFFER; //TODO: may be moved to private field
    char* buffer = new char[bufferSize];
    size_t read = recv(socket, buffer, bufferSize, 0); //Blocking
    if (read<0) {
        delete[] buffer;
        return NULL;
    }
    return buffer;
}