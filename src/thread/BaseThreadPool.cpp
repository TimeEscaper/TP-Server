#include "../../include/thread/BaseThreadPool.h"

BaseThreadPool::BaseThreadPool() { };

BaseThreadPool::~BaseThreadPool() {
    cancelThreads();
    for (int i = 0; i < threads.size(); i++) {
        delete threads[i];
    }
}

bool BaseThreadPool::launchThreads() {
    bool success;
    for (int i = 0; i < threads.size(); i++) {
        success = threads[i]->launch();
        if (!success) {
            return success;
        }
    }
    return success;
}

void BaseThreadPool::cancelThreads() {
    for (int i = 0; i < threads.size(); i++) {
        threads[i]->cancel();
    }
}

AbstractThreadHandler* BaseThreadPool::getFreeThread() {
    while (true) {
        for (int i = 0; i < threads.size(); i++) {
            if (threads[i]->isAvailable()) {
                return threads[i];
            }
        }
    }
}