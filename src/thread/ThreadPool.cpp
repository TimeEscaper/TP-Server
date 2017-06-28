#include <unistd.h>
#include <pthread.h>
#include <stdexcept>
#include <bits/sigthread.h>
#include "../../include/thread/ThreadPool.h"
#include "../../include/logging/log.h"

ThreadPool::ThreadPool(size_t size, int ncpu) {
    int totalCpu = (int)sysconf(_SC_NPROCESSORS_ONLN);
    if ((size < 0) || (ncpu < -1)) {
        throw new std::runtime_error("Invalid thread pool input parameters!");
    }
    if (ncpu > totalCpu) {
        throw new std::runtime_error("Ncpu larger than available CPUs!");
    }

    if (ncpu == MAX_NCPU) {
        usedCpuCount = totalCpu;
    } else {
        usedCpuCount = ncpu;
    }
    if (size == AUTO_POOL_SIZE) {
        poolSize = totalCpu + 1;
    } else {
        poolSize = size;
    }

    int id = 0;
    int cpu = 0;
    for (int i = 0; i < poolSize; i++) {
        if (usedCpuCount != DEFAULT_AFFINITY) {
            threads.push_back(new PooledThread(id, cpu, this));
            id++;
            cpu++;
            if (cpu >= usedCpuCount) {
                cpu = 0;
            }
        } else {
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
    pthread_cond_signal(&threadCond);
    pthread_mutex_unlock(&tasksQueue.mutex);
}

void ThreadPool::cancelThreads() {
    pthread_mutex_lock(&tasksQueue.mutex);
    stop = true;
    pthread_mutex_unlock(&tasksQueue.mutex);
    pthread_cond_broadcast(&threadCond);
}

void *ThreadPool::PooledThread::pthreadWrap(void *object) {
    reinterpret_cast<PooledThread*>(object)->threadLoop();
}

ThreadPool::PooledThread::PooledThread(int id, ThreadPool *owner) {
    this->id = id;
    this->owner = owner;

    int error = pthread_create(&pthread, NULL, PooledThread::pthreadWrap, this);
    if (error != 0) {
        throw new std::runtime_error("Error creating thread, thread id: " + std::to_string(id));
    }

    error = pthread_detach(pthread);
    if (error != 0) {
        throw new std::runtime_error("Error detaching thread, thread id: " + std::to_string(id));
    }
}

ThreadPool::PooledThread::PooledThread(int id, int cpu, ThreadPool *owner) {
    this->id = id;
    this->owner = owner;

    int error = pthread_create(&pthread, NULL, PooledThread::pthreadWrap, this);
    if (error != 0) {
        throw new std::runtime_error("Error creating thread, thread id: " + std::to_string(id));
    }

    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);
    CPU_SET(cpu, &cpuSet);
    error = pthread_setaffinity_np(pthread, sizeof(cpu_set_t), &cpuSet);
    if (error != 0) {
        throw new std::runtime_error("Error setting CPU affinity, thread id: " + std::to_string(id) +
            ", CPU: " + std::to_string(cpu));
    }

    error = pthread_detach(pthread);
    if (error != 0) {
        throw new std::runtime_error("Error detaching thread, thread id: " + std::to_string(id));
    }
}

int ThreadPool::PooledThread::getId() {
    return id;
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