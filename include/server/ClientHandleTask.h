//
// Created by sibirsky on 27.06.17.
//

#ifndef TP_SERVER_CLIENTHANDLETASK_H
#define TP_SERVER_CLIENTHANDLETASK_H


#include "../thread/IThreadTask.h"
#include "ClientHandler.h"

class ClientHandleTask : IThreadTask{
public:
    ClientHandleTask(ClientHandler **client);
    ~ClientHandleTask();
    void execute();

private:
    ClientHandler *client;
};


#endif //TP_SERVER_CLIENTHANDLETASK_H
