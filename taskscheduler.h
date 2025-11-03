#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "task.h"
#include <atomic>
#include <mutex>
#include <vector>
#include <thread>
#include <list>


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
    void dependencyActualization();
    std::unique_ptr<Task> getWaitingTaskToExecute();

private:

    std::vector<std::unique_ptr<Task>> tasks_;
    std::list<std::unique_ptr<Task>> waitingQueue_;

    int currrentPriority_;

    std::thread startExecutionThread_;
    std::thread submitTaskThread_;

    std::atomic_bool onPending_;
    std::mutex tasksMutex_;

    Task::TaskId executedTaskId_;


};

#endif // TASKSCHEDULER_H
