#include "task.h"

#include <iostream>
#include <thread>

using namespace std;

Task::TaskId Task::taskIdCounter_ = 1;

Task::Task(Priority priority)
{
    taskId_ = taskIdCounter_++;     // maybe % 0xffffffffff
    priority_ = priority;
}

GeneralTask::GeneralTask(Priority priority) : Task(priority)
{
}

Task::TaskId GeneralTask::getId() const
{
    return taskId_;
}

Task::Priority GeneralTask::getPriority() const
{
    return priority_;
}

void GeneralTask::execute()
{
    thread th = thread(&GeneralTask::executeTask, this);
    th.join();
}

void GeneralTask::setOverridingTask(const TaskId taskId)
{
    dependencies_.emplace(taskId);
}

void GeneralTask::removeDependentTask(const TaskId taskId)
{
    dependencies_.erase(taskId);
}

bool GeneralTask::isIndependent()
{
    return dependencies_.empty();
}

SecurityTask::SecurityTask(Priority priority) : GeneralTask(priority)
{

}

void SecurityTask::executeTask()
{
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
    std::cerr << "\n Execution of SecurityTask nr: " << taskId_ << "  priority: " << priority_;
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
}

ControlTask::ControlTask(Priority priority) : GeneralTask(priority)
{

}

void ControlTask::executeTask()
{
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
    std::cerr << "\n Execution of ControlTask nr: " << taskId_ << "  priority: " << priority_;
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
}

ProcessTask::ProcessTask(Priority priority) : GeneralTask(priority)
{
}

void ProcessTask::executeTask()
{
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
    std::cerr << "\n Execution of ProcessTask nr: " << taskId_ << "  priority: " << priority_;
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
}

