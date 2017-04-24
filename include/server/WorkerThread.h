#ifndef TP_SERVER_WORKERTHREAD_H
#define TP_SERVER_WORKERTHREAD_H


#include "../thread/AbstractThreadHandler.h"
#include "ClientHandler.h"

class WorkerThread : AbstractThreadHandler {
protected:
    ClientHandler *client;
public:
    ClientHandler* getCurrentClient();
    void processClient(ClientHandler **newClient);
};


#endif //TP_SERVER_WORKERTHREAD_H
