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
    int getSocket();
    long sendRaw(const char* data);
    void receiveRaw(long* received, char** receivePtr);
};


#endif //TP_SERVER_CLIENTHANDLER_H
