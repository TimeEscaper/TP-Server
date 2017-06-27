#ifndef TP_SERVER_CLIENTHANDLETASK_H
#define TP_SERVER_CLIENTHANDLETASK_H


#include <string>
#include "../thread/IThreadTask.h"
#include "ClientHandler.h"

class ClientHandleTask : public IThreadTask {
public:
    ClientHandleTask(ClientHandler **client, std::string& rootDir);
    ~ClientHandleTask();
    void execute();

private:
    ClientHandler *client;
    std::string rootDir;
};


#endif //TP_SERVER_CLIENTHANDLETASK_H
