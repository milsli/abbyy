#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "task.h"
#include <atomic>
#include <vector>
#include <thread>

class TaskScheduler
{
public:
    TaskScheduler();
    ~TaskScheduler();
    void submitTask(std::unique_ptr<Task> task);
    void start();
    void stop();


private:
    void startExecutionTask();

private:
    // Your implementation here

    std::vector<std::unique_ptr<Task>> tasks_;
    int currrentPriority_;
    std::thread startExecutionThread;
    std::atomic_bool onPending_;


};

#endif // TASKSCHEDULER_H
