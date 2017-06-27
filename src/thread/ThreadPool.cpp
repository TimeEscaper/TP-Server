#include <unistd.h>
#include <pthread.h>
#include "../../include/thread/ThreadPool.h"

#define DEFAULT_CPU_AFFINITY -1

ThreadPool::ThreadPool(size_t size) {
    poolSize = size;
    //TODO: check on null
    usedCpuCount = (int)sysconf(_SC_NPROCESSORS_ONLN);
    int id = 0;
    int cpu = 0;
    for (int i = 0; i < poolSize; i++) {
        //TODO: throw exception if error
        threads.push_back(new PooledThread(id, cpu, this));
        id++;
        cpu++;
        if (cpu >= usedCpuCount) {
            cpu = 0;
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

    int id = 0;
    int cpu = 0;
    for (int i = 0; i < poolSize; i++) {
        if (usedCpuCount != DEFAULT_CPU_AFFINITY) {
            //TODO: throw exception if error
            threads.push_back(new PooledThread(id, cpu, this));
            id++;
            cpu++;
            if (cpu >= usedCpuCount) {
                cpu = 0;
            }
        } else {
            //TODO: throw exception if error
            threads.push_back(new PooledThread(id, this));
            id++;
        }
    }

}

ThreadPool::~ThreadPool() {
    if (!stop) {
        cancelThreads();
    }
    for (int i = 0; i < poolSize; i++) {
        delete threads[i];
    }
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
        threads[i]->join();
    }
}

void *ThreadPool::PooledThread::pthreadWrap(void *object) {
    reinterpret_cast<PooledThread*>(object)->threadLoop();
}

ThreadPool::PooledThread::PooledThread(int id, ThreadPool *owner) {
    this->id = id;
    this->owner = owner;

    //TODO: throw exception
    int error = pthread_create(&pthread, NULL, PooledThread::pthreadWrap, this);
}

ThreadPool::PooledThread::PooledThread(int id, int cpu, ThreadPool *owner) {
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


void ThreadPool::PooledThread::join() {
    pthread_join(pthread, NULL);
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