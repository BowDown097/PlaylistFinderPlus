#ifndef TASKEXECUTOR_H
#define TASKEXECUTOR_H
#include "runcontrollabletask.h"

class TaskExecutor
{
public:
    template<class T>
    static QFuture<T> run(ControllableTask<T>* task)
    {
        return (new RunControllableTask<T>(task))->start();
    }
};

#endif // TASKEXECUTOR_H
