#ifndef TP_SERVER_THREADHANDLER_H
#define TP_SERVER_THREADHANDLER_H

#include <pthread.h>

typedef enum {
    ERROR,
    FREE,
    BUSY,
    CANCELLED
} ThreadState;

class AbstractThreadHandler {
protected:
    pthread_t pthread;
    struct {
        ThreadState state;
        pthread_mutex_t mutex;
    } state;
    void setState(ThreadState newState);
    void *threadRoutine(void *arg);
    virtual void threadWork();

public:
    AbstractThreadHandler();
    ~AbstractThreadHandler();
    void cancel();
    ThreadState getState();
};


#endif //TP_SERVER_THREADHANDLER_H
