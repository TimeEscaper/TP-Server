#ifndef TP_SERVER_THREADHANDLER_H
#define TP_SERVER_THREADHANDLER_H

#include <pthread.h>

typedef enum {
    INITED,
    FAILED,
    FREE,
    BUSY,
    CANCELLED
} ThreadState;

class AbstractThreadHandler {
protected:
    pthread_t pthread;
    int cpu = -1;
    ThreadState state;
    static void *threadLaunch(void *object);
    virtual void threadWork();

public:
    AbstractThreadHandler();
    AbstractThreadHandler(int cpu);
    ~AbstractThreadHandler();
    bool launch();
    void cancel();
    ThreadState getState();
    bool isAvailable();
};


#endif //TP_SERVER_THREADHANDLER_H
