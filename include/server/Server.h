#ifndef TP_SERVER_SERVER_H
#define TP_SERVER_SERVER_H

#include "ClientHandler.h"

#define DEFAULT_BACKLOG_SIZE 10

//TODO: Move to static or protected field in server or client (may need to bind monitor)
#define ROOT_PATH "/home/sibirsky/static"

class Server {
private:
    int port;
    int socket;
    bool isWorking = false;
    void handleClient(ClientHandler client); //Temp method

public:
    Server(int port);
    ~Server();
    int getPort();
    void start();
    void stop();
    void restart();
};


#endif //TP_SERVER_SERVER_H
