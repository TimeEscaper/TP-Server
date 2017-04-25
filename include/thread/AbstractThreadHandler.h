#ifndef TP_SERVER_THREADHANDLER_H
#define TP_SERVER_THREADHANDLER_H

#include <pthread.h>

typedef enum {
    INITED,
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
    static void *threadLaunch(void *object);
    virtual void threadWork();

public:
    AbstractThreadHandler();
    ~AbstractThreadHandler();
    bool launch();
    void cancel();
    ThreadState getState();
    virtual bool isAvalible();
};


#endif //TP_SERVER_THREADHANDLER_H
