#ifndef TASKCONTROL_H
#define TASKCONTROL_H
#include <QFutureInterfaceBase>

class TaskControl
{
public:
    explicit TaskControl(QFutureInterfaceBase* f) : fu(f) {}
    bool shouldRun() const { return !fu->isCanceled(); }
private:
    QFutureInterfaceBase* fu;
};

#endif // TASKCONTROL_H
