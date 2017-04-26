#ifndef TP_SERVER_SERVER_H
#define TP_SERVER_SERVER_H

#include <thread>
#include "ClientHandler.h"
#include "WorkerThread.h"
#include "WorkerThreadPool.h"

#define DEFAULT_BACKLOG_SIZE 10
#define NCPU_MAX 0
#define DEFAULT_POOL_SIZE 8

class Server {
private:
    int port;
    int socket;
    std::string rootDir;
    bool isWorking = false;
    void cleanUp();
    WorkerThreadPool *threadPool;

public:
    Server(int port, const std::string &rootDir, int threadPoolSize, int ncpu);
    ~Server();
    int getPort();
    std::string getRootDir();
    int start(); //int type for using with thread
    void stop();
    //void restart();
};


#endif //TP_SERVER_SERVER_H
