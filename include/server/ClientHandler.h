#ifndef TP_SERVER_CLIENTHANDLER_H
#define TP_SERVER_CLIENTHANDLER_H


#include <cstddef>

class ClientHandler {
private:
    int socket;

public:
    ClientHandler(int socket);
    ~ClientHandler();
    int getSocket();
    void send(const char* data);
    void receive(char* data);
};


#endif //TP_SERVER_CLIENTHANDLER_H
