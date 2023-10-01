#ifndef RUNCONTROLLABLETASK_H
#define RUNCONTROLLABLETASK_H
#include "controllabletask.h"
#include <QRunnable>
#include <QThreadPool>

template<typename T>
class RunControllableTask : public QFutureInterface<T>, public QRunnable
{
public:
    explicit RunControllableTask(ControllableTask<T>* task) : task(task) {}
    virtual ~RunControllableTask() { delete task; }

    inline T result() const requires (!std::same_as<T, void>)
    {
        return this->resultReference(0);
    }

    void run()
    {
        if (this->isCanceled())
        {
            this->reportFinished();
            return;
        }

        TaskControl control(this);
        if constexpr (std::is_same_v<T, void>)
        {
            task->run(control);
        }
        else
        {
            T result = task->run(control);
            if (!this->isCanceled())
                this->reportResult(result);
        }

        this->reportFinished();
    }

    QFuture<T> start()
    {
        this->setRunnable(this);
        this->reportStarted();
        QFuture<T> future = this->future();
        QThreadPool::globalInstance()->start(this);
        return future;
    }
private:
    ControllableTask<T>* task;
};

#endif // RUNCONTROLLABLETASK_H
