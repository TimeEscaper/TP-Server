#ifndef TP_SERVER_SERVER_H
#define TP_SERVER_SERVER_H

#include <thread>
#include "ClientHandler.h"
#include "WorkerThread.h"

#define DEFAULT_BACKLOG_SIZE 10


class Server {
private:
    int port;
    int socket;
    std::string rootDir;
    bool isWorking = false;
    void cleanUp();
    WorkerThread *worker;

public:
    Server(int port, const std::string &rootDir);
    ~Server();
    int getPort();
    std::string getRootDir();
    int start(); //int type for using with thread
    void stop();
    //void restart();
};


#endif //TP_SERVER_SERVER_H
