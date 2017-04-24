#ifndef TP_SERVER_WORKERTHREAD_H
#define TP_SERVER_WORKERTHREAD_H


#include "../thread/AbstractThreadHandler.h"
#include "ClientHandler.h"

class WorkerThread : AbstractThreadHandler {
protected:
    ClientHandler *client;
    void threadWork();
    char* rootDir = new char;
    struct {
        bool hasWork;
        pthread_mutex_t mutex;
        pthread_cond_t cond;
    } workState;
public:
    WorkerThread(const char* rootDir);
    ~WorkerThread();
    char* getRootDir();
    void handleClient(ClientHandler **newClient);
    bool isAvalible();
};


#endif //TP_SERVER_WORKERTHREAD_H
