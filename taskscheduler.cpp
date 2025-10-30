#include "taskscheduler.h"

using namespace std;

TaskScheduler::TaskScheduler()
    : currrentPriority_{ 1 }
{
    onPending_ = false;
}

TaskScheduler::~TaskScheduler()
{
    // if (startExecutionThread.joinable())
    //     startExecutionThread.join();
}

void TaskScheduler::submitTask(unique_ptr<Task> task)
{
    Task::Priority newTaskPriority = task->getPriority();

    if (currrentPriority_ > newTaskPriority) {
        currrentPriority_ = newTaskPriority;
    }

    tasks_.emplace_back(std::move(task));
}

void TaskScheduler::start()
{
    onPending_ = true;
    startExecutionThread = thread(&TaskScheduler::startExecutionTask, this);
}

void TaskScheduler::stop()
{
    onPending_ = false;

    if (startExecutionThread.joinable())
        startExecutionThread.join();
}

void TaskScheduler::startExecutionTask()
{
    vector<unique_ptr<Task>>::iterator priorityIterator;

    while (onPending_) {
        while (!tasks_.empty()) {
            priorityIterator = std::find_if(tasks_.begin(), tasks_.end(), [&](const unique_ptr<Task> &task)
                                            {
                                                return task->getPriority() == currrentPriority_;
                                            });

            if (priorityIterator != tasks_.end()) {
                swap(*priorityIterator, tasks_.back());
                tasks_.back()->execute();
                tasks_.pop_back();
            } else {
                ++currrentPriority_;
            }
        }
    }
}
