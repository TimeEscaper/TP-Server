#include "../../include/thread/AbstractThreadHandler.h"
#include "../../include/helpers/utils.h"

AbstractThreadHandler::AbstractThreadHandler() {
    state = {ThreadState::INITED, PTHREAD_MUTEX_INITIALIZER};
}

AbstractThreadHandler::~AbstractThreadHandler() {
    cancel();
}

bool AbstractThreadHandler::launch() {
    pthread_mutex_lock(&state.mutex);
    if (state.state != ThreadState::INITED) {
        pthread_mutex_unlock(&state.mutex);
        return false;
    }
    int error = pthread_create(&pthread, NULL, threadLaunch, this);
    if (error != 0) {
        state.state = ThreadState::ERROR;
        pthread_mutex_unlock(&state.mutex);
        return false;
    }
    state.state = ThreadState::FREE;
    pthread_mutex_unlock(&state.mutex);
    return true;
}

void AbstractThreadHandler::setState(ThreadState newState) {
    pthread_mutex_lock(&state.mutex);
    state.state = newState;
    pthread_mutex_unlock(&state.mutex);
}

ThreadState AbstractThreadHandler::getState() {
    pthread_mutex_lock(&state.mutex);
    ThreadState resultState = state.state;
    pthread_mutex_unlock(&state.mutex);
    return resultState;
}


void AbstractThreadHandler::cancel() {
    pthread_mutex_lock(&state.mutex);
    if ((state.state == ThreadState::BUSY) || (state.state == ThreadState::FREE)) {
        state.state = ThreadState::CANCELLED;
        int error = pthread_cancel(pthread);
        if (error != 0) {
            utils::log("Unable to cancel thread!");
        }
        error = pthread_detach(pthread);
        if (error != 0) {
            utils::log("Unable to detach thread!");
        }
    }
    pthread_mutex_unlock(&state.mutex);
}

void* AbstractThreadHandler::threadLaunch(void *object) {
    reinterpret_cast<AbstractThreadHandler*>(object)->threadWork();
}

void AbstractThreadHandler::threadWork() { };

bool AbstractThreadHandler::isAvalible() { };