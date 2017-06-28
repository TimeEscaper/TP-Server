#ifndef TP_SERVER_THREADPOOL_H
#define TP_SERVER_THREADPOOL_H


#include <sys/types.h>
#include <queue>
#include "IThreadTask.h"

#define AUTO_POOL_SIZE 0
#define MAX_NCPU 0
#define DEFAULT_AFFINITY -1

class ThreadPool {
public:
    ThreadPool(size_t  size, int ncpu);
    ~ThreadPool();
    void pushTask(IThreadTask **task);

private:
    class PooledThread;

    size_t poolSize;
    int usedCpuCount;
    bool stop = false;
    std::vector<ThreadPool::PooledThread*> threads;
    pthread_cond_t threadCond = PTHREAD_COND_INITIALIZER;
    struct {
        std::queue<IThreadTask*> queue;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    } tasksQueue;

    void cancelThreads();

    friend ThreadPool::PooledThread;

    class PooledThread {
    public:
        PooledThread(int id, ThreadPool *owner);
        PooledThread(int id, int cpu, ThreadPool *owner);

        int getId();

    private:
        int id;
        pthread_t pthread;
        ThreadPool *owner;

        static void *pthreadWrap(void *object);

        void threadLoop();
    };
};


#endif //TP_SERVER_THREADPOOL_H
