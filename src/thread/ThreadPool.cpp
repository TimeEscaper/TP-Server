#include <unistd.h>
#include <pthread.h>
#include "../../include/thread/ThreadPool.h"

#define DEFAULT_CPU_AFFINITY -1

int ThreadPool::createThread(ThreadPool *owner) {
    PooledThread *thread = new PooledThread(allThreads.size() + 1, owner);
    allThreads.push_back(thread);
    return thread->getId();
}

int ThreadPool::createThread(ThreadPool *owner, int cpu) {
    PooledThread *thread = new PooledThread(allThreads.size() + 1, cpu, owner);
    allThreads.push_back(thread);
    return thread->getId();
}

ThreadPool::ThreadPool(size_t size) {
    poolSize = size;
    //TODO: check on null
    usedCpuCount = (int)sysconf(_SC_NPROCESSORS_ONLN);
    ownedThreads = new int[poolSize];
    int id = 0;
    int cpu = 0;
    for (int i = 0; i < poolSize; i++) {
        id = ThreadPool::createThread(this, cpu);
        //TODO: throw exception if error
        if (id != -1) {
            ownedThreads[i] = id;
            cpu++;
            if (cpu >= usedCpuCount) {
                cpu = 0;
            }
        }
    }
}

ThreadPool::ThreadPool(size_t size, int ncpu) {
    poolSize = size;
    //TODO: check on null
    if (ncpu == DEFAULT_CPU_AFFINITY) {
        usedCpuCount = DEFAULT_CPU_AFFINITY;
    } else {
        long cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
        if ((ncpu > 0) && (ncpu <= cpuCount)) {
            usedCpuCount = ncpu;
        } else {
            //TODO: Throw exception
            usedCpuCount = (int)cpuCount;
        }
    }

    ownedThreads = new int[poolSize];
    int id = 0;
    int cpu = 0;
    for (int i = 0; i < poolSize; i++) {
        if (usedCpuCount != DEFAULT_CPU_AFFINITY) {
            id = ThreadPool::createThread(this, cpu);
            //TODO: throw exception if error
            if (id != -1) {
                ownedThreads[i] = id;
                cpu++;
                if (cpu >= usedCpuCount) {
                    cpu = 0;
                }
            }
        } else {
            id = ThreadPool::createThread(this, cpu);
            //TODO: throw exception if error
            if (id != -1) {
                ownedThreads[i] = id;
            }
        }
    }

}

ThreadPool::~ThreadPool() {
    if (!stop) {
        cancelThreads();
    }
    for (int i = 0; i < poolSize; i++) {
        delete ThreadPool::allThreads[ownedThreads[i]];
    }
    delete[] ownedThreads;
}

void ThreadPool::pushTask(IThreadTask **task) {
    pthread_mutex_lock(&tasksQueue.mutex);
    tasksQueue.queue.push(*task);
    pthread_mutex_unlock(&tasksQueue.mutex);
    pthread_cond_signal(&threadCond);
}

void ThreadPool::cancelThreads() {
    pthread_mutex_lock(&tasksQueue.mutex);
    stop = true;
    pthread_mutex_unlock(&tasksQueue.mutex);
    pthread_cond_broadcast(&threadCond);

    for (int i = 0; i < poolSize; i++) {
        ThreadPool::allThreads[ownedThreads[i]]->join();
    }
}

void *ThreadPool::PooledThread::pthreadWrap(void *object) {
    reinterpret_cast<PooledThread*>(object)->threadLoop();
}

ThreadPool::PooledThread::PooledThread(int id, const ThreadPool *owner) {
    this->id = id;
    this->owner = owner;

    //TODO: throw exception
    int error = pthread_create(&pthread, NULL, PooledThread::pthreadWrap, this);
}

ThreadPool::PooledThread::PooledThread(int id, int cpu, const ThreadPool *owner) {
    this->id = id;
    this->owner = owner;

    //TODO: throw exception
    int error = pthread_create(&pthread, NULL, PooledThread::pthreadWrap, this);

    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);
    CPU_SET(cpu, &cpuSet);
    //TODO: check on error
    error = pthread_setaffinity_np(pthread, sizeof(cpu_set_t), &cpuSet);
}

int ThreadPool::PooledThread::getId() {
    return id;
}

bool ThreadPool::PooledThread::isJoined() {
    return joined;
}

void ThreadPool::PooledThread::join() {
    pthread_join(joined, NULL);
    joined = true;
}

void ThreadPool::PooledThread::threadLoop() {
    while (true) {
        pthread_mutex_lock(&owner->tasksQueue.mutex);
        while ((!owner->stop) && (owner->tasksQueue.queue.empty())) {
            pthread_cond_wait(&owner->threadCond, &owner->tasksQueue.mutex);
        }

        if (owner->stop) {
            pthread_mutex_unlock(&owner->tasksQueue.mutex);
            return;
        }

        IThreadTask* task = owner->tasksQueue.queue.front();
        owner->tasksQueue.queue.pop();
        pthread_mutex_unlock(&owner->tasksQueue.mutex);

        task->execute();
        delete task;
    }
}
#pragma clang diagnostic pop