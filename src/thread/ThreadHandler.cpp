#include "../../include/thread/ThreadHandler.h"
#include "../../include/helpers/utils.h"

ThreadHandler::ThreadHandler() {
    state = ThreadState::INITED;
}

ThreadHandler::~ThreadHandler() {
    cancel();
    cleanup();
}

void ThreadHandler::setState(ThreadState newState) {
    state = newState;
}

ThreadState ThreadHandler::getState() {
    return state;
}

void ThreadHandler::run() {
    if (state == ThreadState::INITED) {
        setState(ThreadState::FREE);
        error = pthread_create(&pthread, NULL, threadRoutine, NULL);
        if (error != 0) {
            setState(ThreadState::NOT_STARTED);
        }
    }
}

void ThreadHandler::cancel() {
    if ((getState() == ThreadState::BUSY) || (getState() == ThreadState::FREE)) {
        setState(ThreadState::CANCELLED);
        error = pthread_cancel(pthread);
        if (error != 0) {
            utils::log("Unable to cancel thread!");
        }
        error = pthread_detach(pthread);
        if (error != 0) {
            utils::log("Unable to detach thread!");
        }
    }
}

void *ThreadHandler::threadRoutine(void *arg) {
    threadWork();
}