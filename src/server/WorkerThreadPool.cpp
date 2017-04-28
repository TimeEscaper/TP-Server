#include <unistd.h>
#include <iostream>
#include "../../include/server/WorkerThreadPool.h"
#include "../../include/server/WorkerThread.h"

WorkerThreadPool::WorkerThreadPool(std::string rootDir, int size, int ncpu) {
    this->size = size;
    long cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
    if ((ncpu > 0) && (ncpu <= cpuCount)) {
        this->ncpu = ncpu;
    } else {
        this->ncpu = cpuCount;
    }
    this->rootDir = rootDir;
    long cpu = 0;
    for (int i = 0; i < size; i++) {
        threads.push_back(new WorkerThread(rootDir, i, cpu));
        cpu++;
        if (cpu >= this->ncpu) {
            cpu = 0;
        }
    }
}

WorkerThreadPool::WorkerThreadPool(std::string rootDir, int size) {
    this->size = size;
    this->ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    this->rootDir = rootDir;
    for (int i = 0; i < size; i++) {
        threads.push_back(new WorkerThread(rootDir, i));
    }
    long cpu = 0;
    for (int i = 0; i < size; i++) {
        threads.push_back(new WorkerThread(rootDir, i, cpu));
        cpu++;
        if (cpu >= this->ncpu) {
            cpu = 0;
        }
    }
}