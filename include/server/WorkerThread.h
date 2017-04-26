#ifndef TP_SERVER_WORKERTHREAD_H
#define TP_SERVER_WORKERTHREAD_H

#include <pthread.h>
#include "../thread/AbstractThreadHandler.h"
#include "ClientHandler.h"
#include "../helpers/MimeUtil.h"

class WorkerThread : public AbstractThreadHandler {
protected:
    ClientHandler *client;
    void threadWork();
    std::string rootDir;
    int id;
    utils::MimeUtil mimeUtil;
    void processClient();
    struct {
        bool hasWork;
        pthread_mutex_t mutex;
        pthread_cond_t cond;
    } workState;
public:
    WorkerThread(const std::string &rootDir, int id);
    ~WorkerThread();
    std::string getRootDir();
    void handleClient(ClientHandler **newClient);
    bool isAvailable();
};


#endif //TP_SERVER_WORKERTHREAD_H
