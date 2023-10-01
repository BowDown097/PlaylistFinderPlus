#ifndef CONTROLLABLETASK_H
#define CONTROLLABLETASK_H
#include "taskcontrol.h"

template<class T>
class ControllableTask
{
public:
    virtual ~ControllableTask() {}
    virtual T run(const TaskControl& control) = 0;
};

#endif // CONTROLLABLETASK_H
