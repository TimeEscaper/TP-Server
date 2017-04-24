#include "../../include/thread/AbstractThreadHandler.h"
#include "../../include/helpers/utils.h"

AbstractThreadHandler::AbstractThreadHandler() {
    state = ThreadState::INITED;
}

AbstractThreadHandler::~AbstractThreadHandler() {
    cancel();
    cleanup();
}

void AbstractThreadHandler::setState(ThreadState newState) {
    state = newState;
}

ThreadState AbstractThreadHandler::getState() {
    return state;
}

void AbstractThreadHandler::run() {
    if (state == ThreadState::INITED) {
        setState(ThreadState::FREE);
        error = pthread_create(&pthread, NULL, threadRoutine, NULL);
        if (error != 0) {
            setState(ThreadState::NOT_STARTED);
        }
    }
}

void AbstractThreadHandler::cancel() {
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

void *AbstractThreadHandler::threadRoutine(void *arg) {
    threadWork();
}