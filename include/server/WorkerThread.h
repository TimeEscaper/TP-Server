#ifndef TP_SERVER_WORKERTHREAD_H
#define TP_SERVER_WORKERTHREAD_H


#include "../thread/AbstractThreadHandler.h"
#include "ClientHandler.h"

class WorkerThread : public AbstractThreadHandler {
protected:
    ClientHandler *client;
    void threadWork();
    std::string rootDir;
    void processClient();
    struct {
        bool hasWork;
        pthread_mutex_t mutex;
        pthread_cond_t cond;
    } workState;
public:
    WorkerThread(const std::string &rootDir);
    ~WorkerThread();
    std::string getRootDir();
    void handleClient(ClientHandler **newClient);
    bool isAvailable();
};


#endif //TP_SERVER_WORKERTHREAD_H
