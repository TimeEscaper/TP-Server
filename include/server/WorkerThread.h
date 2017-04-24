#ifndef TP_SERVER_WORKERTHREAD_H
#define TP_SERVER_WORKERTHREAD_H


#include "../thread/AbstractThreadHandler.h"
#include "ClientHandler.h"

class WorkerThread : AbstractThreadHandler {
protected:
    ClientHandler *client;
    void threadWork();
    char* rootDir = new char;
public:
    WorkerThread(const char* rootDir);
    char* getRootDir();
    void processClient(ClientHandler **newClient);
};


#endif //TP_SERVER_WORKERTHREAD_H
