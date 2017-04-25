#ifndef TP_SERVER_CLIENTHANDLER_H
#define TP_SERVER_CLIENTHANDLER_H

#include <cstddef>

#define DEFAULT_REQUEST_BUFFER 1024

class ClientHandler {
private:
    int socket;

public:
    ClientHandler(int socket);
    ~ClientHandler();
    long sendRaw(const std::string &data);
    std::string receiveRaw();
};


#endif //TP_SERVER_CLIENTHANDLER_H
