#ifndef TP_SERVER_THREADPOOL_H
#define TP_SERVER_THREADPOOL_H

#include <vector>
#include "AbstractThreadHandler.h"

class BaseThreadPool {
protected:
    int size;
    std::vector<AbstractThreadHandler*> threads;

public:
    BaseThreadPool();
    ~BaseThreadPool();
    bool launchThreads();
    void cancelThreads();
    AbstractThreadHandler* getFreeThread();
};


#endif //TP_SERVER_THREADPOOL_H
