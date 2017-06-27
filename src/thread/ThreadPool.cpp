#include <unistd.h>
#include <pthread.h>
#include "../../include/thread/ThreadPool.h"

ThreadPool::ThreadPool(size_t size) {
    poolSize = size;
    //TODO: check on null
    usedCpuCount = (int)sysconf(_SC_NPROCESSORS_ONLN);
    threads = new pthread_t[poolSize];
    int error = 0;
    int cpu = 0;
    for (int i = 0; i < poolSize; i++) {
        error = pthread_create(&threads[i], NULL, threadLoop, this);
        //TODO: throw exception if error
        if (error == 0) {
            if (usedCpuCount != DEFAULT_CPU_AFFINITY) {
                cpu_set_t cpuSet;
                CPU_ZERO(&cpuSet);
                CPU_SET(cpu, &cpuSet);
                //TODO: log if error
                error = pthread_setaffinity_np(threads[i], sizeof(cpuSet), &cpuSet);
            }
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
    if (ncpu == -1) {
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

    threads = new pthread_t[poolSize];
    int error = 0;
    int cpu = 0;
    for (int i = 0; i < poolSize; i++) {
        error = pthread_create(&threads[i], NULL, threadLoop, this);
        //TODO: throw exception if error
        if (error == 0) {
            if (usedCpuCount != DEFAULT_CPU_AFFINITY) {
                cpu_set_t cpuSet;
                CPU_ZERO(&cpuSet);
                CPU_SET(cpu, &cpuSet);
                //TODO: log if error
                error = pthread_setaffinity_np(threads[i], sizeof(cpuSet), &cpuSet);
            }
            cpu++;
            if (cpu >= usedCpuCount) {
                cpu = 0;
            }
        }
    }
}

ThreadPool::~ThreadPool() {
    cancelThreads();
    delete[] threads;
}

void ThreadPool::pushTask(IThreadTask **task) {
    pthread_mutex_lock(&tasksQueue.mutex);
    tasksQueue.queue.push(*task);
    pthread_mutex_unlock(&tasksQueue.mutex);
    pthread_cond_signal(&threadCond);
}

void ThreadPool::threadLoop() {
    while (true) {
        pthread_mutex_lock(&tasksQueue.mutex);
        while ((!needStop) && (tasksQueue.queue.empty())) {
            pthread_cond_wait(&threadCond, &tasksQueue.mutex);
        }

        if (needStop) {
            pthread_mutex_unlock(&tasksQueue.mutex);
            return;
        }

        IThreadTask* task = tasksQueue.queue.front();
        tasksQueue.queue.pop();
        pthread_mutex_unlock(&tasksQueue.mutex);

        task->execute();
        delete task;
    }
}

void ThreadPool::cancelThreads() {
    pthread_mutex_lock(&tasksQueue.mutex);
    needStop = true;
    pthread_mutex_unlock(&tasksQueue.mutex);
    pthread_cond_broadcast(&threadCond);

    for (int i = 0; i < poolSize; i++) {
        pthread_join(threads[i], NULL);
    }
}

#pragma clang diagnostic pop