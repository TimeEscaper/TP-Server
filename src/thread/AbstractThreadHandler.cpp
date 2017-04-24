#include "../../include/thread/AbstractThreadHandler.h"
#include "../../include/helpers/utils.h"

AbstractThreadHandler::AbstractThreadHandler() {
    state = {ThreadState::INITED, PTHREAD_MUTEX_INITIALIZER};
}

AbstractThreadHandler::~AbstractThreadHandler() {
    cancel();
    cleanup();
}

void AbstractThreadHandler::setState(ThreadState newState) {
    while (pthread_mutex_lock(&state.mutex) != 0) { };
    state.state = newState;
    if (pthread_mutex_unlock(&state.mutex) != 0) {
        utils::log("Unable to unlock mutex!");
    }

}

ThreadState AbstractThreadHandler::getState() {
    return state.state;
}

void AbstractThreadHandler::run() {
    if (state.state == ThreadState::INITED) {
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