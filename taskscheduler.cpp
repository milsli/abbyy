#include "taskscheduler.h"

using namespace std;

TaskScheduler::TaskScheduler()
    : currrentPriority_{ 1 }
{
    onPending_ = false;
}

TaskScheduler::~TaskScheduler()
{
    if (startExecutionThread_.joinable())
        startExecutionThread_.join();
}

void TaskScheduler::submitTask(unique_ptr<Task> task)
{
    submitTaskThread_ = thread(&TaskScheduler::submitTaskTask, this, std::move(task));
    submitTaskThread_.join();
}

void TaskScheduler::start()
{
    onPending_ = true;
    startExecutionThread_ = thread(&TaskScheduler::startExecutionTask, this);
}

void TaskScheduler::stop()
{
    onPending_ = false;

    if (startExecutionThread_.joinable())
        startExecutionThread_.join();
}

void TaskScheduler::startExecutionTask()
{
    vector<unique_ptr<Task>>::iterator priorityIterator;

    unique_ptr<Task> currentTask;

    while (onPending_) {
        bool taskForExecution = false;
        {
            lock_guard lock(tasksMutex_);
            if (!tasks_.empty()) {

                priorityIterator = std::find_if(tasks_.begin(), tasks_.end(), [&](const unique_ptr<Task> &task)
                                                {
                                                    return task->getPriority() == currrentPriority_;
                                                });

                if (priorityIterator != tasks_.end()) {
                    taskForExecution = true;
                    swap(*priorityIterator, tasks_.back());
                    currentTask = std::move(tasks_.back());
                    tasks_.pop_back();
                } else {
                    ++currrentPriority_;
                }
            }
        }
        if (taskForExecution)
            currentTask->execute();
    }
}

void TaskScheduler::submitTaskTask(unique_ptr<Task> task)
{
    lock_guard lock(tasksMutex_);
    Task::Priority newTaskPriority = task->getPriority();
    if (currrentPriority_ > newTaskPriority) {
        currrentPriority_ = newTaskPriority;
    }
    tasks_.emplace_back(std::move(task));
}
