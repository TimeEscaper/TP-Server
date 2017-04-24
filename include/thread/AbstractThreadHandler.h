#ifndef TP_SERVER_THREADHANDLER_H
#define TP_SERVER_THREADHANDLER_H

#include <pthread.h>

typedef enum {
    INITED,
    NOT_STARTED,
    FREE,
    BUSY,
    CANCELLED
} ThreadState;

class AbstractThreadHandler {
protected:
    int error;
    pthread_t pthread;
    struct {
        ThreadState state;
        pthread_mutex_t mutex;
    } state;
    void setState(ThreadState newState);
    void *threadRoutine(void *arg);
    virtual void threadWork();
    virtual void cleanup();
public:
    AbstractThreadHandler();
    ~AbstractThreadHandler();
    void run();
    void cancel();
    ThreadState getState();
};


#endif //TP_SERVER_THREADHANDLER_H
