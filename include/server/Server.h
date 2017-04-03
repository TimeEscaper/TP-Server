#ifndef TP_SERVER_SERVER_H
#define TP_SERVER_SERVER_H

#include <thread>
#include "ClientHandler.h"

#define DEFAULT_BACKLOG_SIZE 10


class Server {
private:
    int port;
    int socket;
    char* rootDir = new char;
    bool isWorking = false;
    void cleanUp();
    void handleClient(ClientHandler client); //Temp method

public:
    Server(int port, const char* rootDir);
    ~Server();
    int getPort();
    char* getRootDir();
    int start(); //int type for using with thread
    void stop();
    void restart();
};


#endif //TP_SERVER_SERVER_H
