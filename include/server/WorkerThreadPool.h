#ifndef TP_SERVER_WORKERTHREADPOOL_H
#define TP_SERVER_WORKERTHREADPOOL_H

#include <string>
#include "../thread/BaseThreadPool.h"
#include "WorkerThread.h"

class WorkerThreadPool : public BaseThreadPool {
protected:
    void addThread();
    std::string rootDir;
    long ncpu;
public:
    WorkerThreadPool(std::string rootDir, int size, int ncpu);
    WorkerThreadPool(std::string rootDir, int size);
};


#endif //TP_SERVER_WORKERTHREADPOOL_H
