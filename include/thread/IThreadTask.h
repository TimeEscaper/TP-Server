#ifndef TP_SERVER_ITASK_H
#define TP_SERVER_ITASK_H


class IThreadTask {
public:
    virtual ~IThreadTask() {};
    virtual void execute() = 0;
};


#endif //TP_SERVER_ITASK_H
