//
// Created by sibirsky on 27.06.17.
//

#ifndef TP_SERVER_THREADPOOL_H
#define TP_SERVER_THREADPOOL_H


#include <sys/types.h>
#include <queue>
#include "IThreadTask.h"

#define DEFAULT_CPU_AFFINITY -1

class ThreadPool {
public
    ThreadPool(size_t size);
    ThreadPool(size_t  size, int ncpu);
    ~ThreadPool();
    void pushTask(IThreadTask **task);

private:
    size_t poolSize;
    int usedCpuCount;
    bool needStop = false;
    pthread_t threads[];

    pthread_cond_t threadCond = PTHREAD_COND_INITIALIZER;
    struct {
        std::queue<IThreadTask*> queue;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    } tasksQueue;

    void threadLoop();
    void cancelThreads();
};


#endif //TP_SERVER_THREADPOOL_H
