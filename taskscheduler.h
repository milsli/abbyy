#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "task.h"
#include <atomic>
#include <mutex>
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
    void submitTaskTask(std::unique_ptr<Task> task);

private:
    // Your implementation here

    std::vector<std::unique_ptr<Task>> tasks_;
    int currrentPriority_;

    std::thread startExecutionThread_;
    std::thread submitTaskThread_;

    std::atomic_bool onPending_;
    std::mutex tasksMutex_;


};

#endif // TASKSCHEDULER_H
