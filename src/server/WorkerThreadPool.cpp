#include "../../include/server/WorkerThreadPool.h"
#include "../../include/server/WorkerThread.h"

WorkerThreadPool::WorkerThreadPool(std::string rootDir, int size, int ncpu) {
    this->size = size;
    this->ncpu = ncpu;
    this->rootDir = rootDir;
    for (int i = 0; i < size; i++) {
        threads.push_back(new WorkerThread(rootDir, i));
    }
}