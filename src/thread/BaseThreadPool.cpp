#include <unistd.h>
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
    if (threads[currentFreeThread]->isAvailable()) {
        int ret = currentFreeThread;
        currentFreeThread++;
        if (currentFreeThread >= threads.size()) {
            currentFreeThread = 0;
        }
        return threads[ret];
    }
    while (true) {
        while (currentFreeThread < threads.size()) {
            if (threads[currentFreeThread]->isAvailable()) {
                return threads[currentFreeThread++];
            }
            currentFreeThread++;
        }
        currentFreeThread = 0;
    }
}