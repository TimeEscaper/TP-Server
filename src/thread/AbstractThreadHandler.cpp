#include <unistd.h>
#include "../../include/thread/AbstractThreadHandler.h"
#include "../../include/helpers/utils.h"

AbstractThreadHandler::AbstractThreadHandler(int cpu) {
    long cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
    //TODO: add exception
    if ((cpu >= 0) && (cpu < cpuCount)) {
        this->cpu = cpu;
    }
    state = ThreadState::INITED;
}

AbstractThreadHandler::AbstractThreadHandler() {
    state = ThreadState::INITED;
}

AbstractThreadHandler::~AbstractThreadHandler() {
    cancel();
}

bool AbstractThreadHandler::launch() {
    if (state != ThreadState::INITED) {
        return false;
    }

    int error = 0;
    error = pthread_create(&pthread, NULL, threadLaunch, this);
    if (error != 0) {
        state = ThreadState::FAILED;
        return false;
    }

    if (cpu != -1) {
        cpu_set_t cpuSet;
        CPU_ZERO(&cpuSet);
        CPU_SET(cpu, &cpuSet);
        error = pthread_setaffinity_np(pthread, sizeof(cpu_set_t), &cpuSet);
        if (error != 0) {
            //utils::log("Unable to set cpu affinity for thread!");
            state = ThreadState::FAILED;
            return false;
        }
    }

    state = ThreadState::FREE;
    return true;
}

ThreadState AbstractThreadHandler::getState() {
    return state;
}


void AbstractThreadHandler::cancel() {
    if (state != ThreadState::CANCELLED) {
        state = ThreadState::CANCELLED;
        int error = pthread_cancel(pthread);
        if (error != 0) {
            utils::log("Unable to cancel thread!");
        }
        error = pthread_detach(pthread);
        if (error != 0) {
            utils::log("Unable to detach thread!");
        }
    }
}

void* AbstractThreadHandler::threadLaunch(void *object) {
    reinterpret_cast<AbstractThreadHandler*>(object)->threadWork();
}

bool AbstractThreadHandler::isAvailable() {
    return state == ThreadState::FREE;
};

void AbstractThreadHandler::threadWork() { };