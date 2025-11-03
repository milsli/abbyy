#include "taskscheduler.h"

using namespace std;

TaskScheduler::TaskScheduler()
    : currrentPriority_{ 1 }
    , executedTaskId_ { 0 }
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

            // dependencyActualization();
            // getWaitingTastToExecute();
            if (currentTask != nullptr)
                taskForExecution = true;

            if (!tasks_.empty() && currentTask == nullptr) {
                priorityIterator = std::find_if(tasks_.begin(), tasks_.end(), [&](const unique_ptr<Task> &task)
                                                {
                                                    return task->getPriority() == currrentPriority_;
                                                });

                if (priorityIterator != tasks_.end()) {

                    if ((*priorityIterator)->isIndependent()) {
                        swap(*priorityIterator, tasks_.back());
                        currentTask = std::move(tasks_.back());
                        taskForExecution = true;
                    } else {
                        // task to waiting queue
                        swap(*priorityIterator, tasks_.back());
                        waitingQueue_.emplace_back(std::move(tasks_.back()));
                    }
                    tasks_.pop_back();
                } else {
                    ++currrentPriority_;
                }
            } else {
                this_thread::sleep_for(chrono::milliseconds{ 100 });
            }
        }
        if (taskForExecution) {
            currentTask->execute();
            executedTaskId_ = currentTask->getId();
        }
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

void TaskScheduler::dependencyActualization()
{
    for (auto &waitingTask: waitingQueue_) {
        waitingTask->removeDependentTask(executedTaskId_);
    }
}

// Queue checking. If there are no dependencies getting task to execute
void TaskScheduler::getWaitingTastToExecute()
{
    auto it = find_if(waitingQueue_.begin(), waitingQueue_.end(), [](const unique_ptr<Task>& task)
            {
                return task->isIndependent();
            });

    if (it != waitingQueue_.end()) {
        tasks_.push_back(std::move(*it));
        waitingQueue_.erase(it);
    }
}
