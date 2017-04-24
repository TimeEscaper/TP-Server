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

typedef struct {
    ThreadState state;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ThreadStateSync;

class AbstractThreadHandler {
protected:
    int error;
    pthread_t pthread;
    ThreadStateSync state = {ThreadState::INITED, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
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
