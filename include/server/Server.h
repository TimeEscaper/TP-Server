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
    static std::string rootDir;
    int port;
    int socket;
    bool isWorking = false;
    void cleanUp();
    WorkerThreadPool *threadPool;

public:
    Server(int port, const std::string &rootDir, int threadPoolSize, int ncpu);
    ~Server();
    int getPort();
    static std::string getRootDir();
    static std::string setRootDir(std::string& rootDir);
    int start(); //int type for using with thread
    void stop();
    //void restart();
};


#endif //TP_SERVER_SERVER_H
