#ifndef TP_SERVER_THREADPOOL_H
#define TP_SERVER_THREADPOOL_H


#include <sys/types.h>
#include <queue>
#include "IThreadTask.h"

class ThreadPool {
public:
    ThreadPool(size_t size);
    ThreadPool(size_t  size, int ncpu);
    ~ThreadPool();
    void pushTask(IThreadTask **task);

private:
    static std::vector<PooledThread*> allThreads;

    size_t poolSize;
    int usedCpuCount;
    bool stop = false;
    pthread_cond_t threadCond = PTHREAD_COND_INITIALIZER;
    struct {
        std::queue<IThreadTask*> queue;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    } tasksQueue;
    int ownedThreads[];

    static int createThread(ThreadPool *owner);
    static int createThread(ThreadPool *owner, int cpu);

    void cancelThreads();

    friend class ThreadPool::PooledThread;

    class PooledThread {
    public:
        PooledThread(int id, const ThreadPool *owner);
        PooledThread(int id, int cpu, const ThreadPool *owner);
        ~PooledThread();
        int getId();
        bool isJoined();
        void join();

    private:
        int id;
        pthread_t pthread;
        bool  joined = false;
        const ThreadPool *owner;

        static void *pthreadWrap(void *object);

        void threadLoop();
    };
};


#endif //TP_SERVER_THREADPOOL_H
