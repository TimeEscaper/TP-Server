#ifndef TP_SERVER_ITHREADHANDLER_H
#define TP_SERVER_ITHREADHANDLER_H

#include <pthread.h>

typedef enum {
    INITED,
    NOT_STARTED,
    FREE,
    BUSY,
    CANCELLED
} ThreadState;

class ThreadHandler {
protected:
    int error;
    pthread_t pthread;
    ThreadState state;
    void setState(ThreadState newState);
    void *threadRoutine(void *arg);
    virtual void threadWork();
    virtual void cleanup();
public:
    ThreadHandler();
    ~ThreadHandler();
    void run();
    void cancel();
    ThreadState getState();
};


#endif //TP_SERVER_ITHREADHANDLER_H
