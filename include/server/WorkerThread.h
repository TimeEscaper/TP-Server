#ifndef TP_SERVER_WORKERTHREAD_H
#define TP_SERVER_WORKERTHREAD_H


#include "../thread/ThreadHandler.h"
#include "ClientHandler.h"

class WorkerThread : ThreadHandler {
protected:
    ClientHandler *client;
public:
    ClientHandler* getCurrentClient();
    void processClient(ClientHandler **newClient);
};


#endif //TP_SERVER_WORKERTHREAD_H
