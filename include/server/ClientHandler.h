#ifndef TP_SERVER_CLIENTHANDLER_H
#define TP_SERVER_CLIENTHANDLER_H

#include <cstddef>

class ClientHandler {
private:
    int socket;

public:
    ClientHandler(int socket);
    ~ClientHandler();
    long sendRaw(const std::string &data);
    long sendRaw(const char *data, ssize_t dataLength);
    std::string receiveRaw();
};


#endif //TP_SERVER_CLIENTHANDLER_H
