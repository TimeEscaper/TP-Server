#ifndef TP_SERVER_SERVER_H
#define TP_SERVER_SERVER_H


#include "../thread/ThreadPool.h"

#define DEFAULT_BACKLOG_SIZE 10

class Server {
public:
    Server(int port, const std::string &rootDir, int threadPoolSize, int ncpu);
    ~Server();

    int start(); //int type for using with thread
    void stop();

private:
    int port;
    int socket;
    std::string rootDir;
    bool isWorking = false;
    ThreadPool *threadPool;

    void cleanUp();
};


#endif //TP_SERVER_SERVER_H
