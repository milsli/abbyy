#include "task.h"

#include <iostream>
#include <thread>

using namespace std;

Task::TaskId Task::taskIdCounter_ = 1;

Task::Task(Priority priority)
{
    taskId_ = taskIdCounter_++;
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

SecurityTask::SecurityTask(Priority priority) : GeneralTask(priority)
{

}

void SecurityTask::execute()
{
    thread th = thread(&SecurityTask::executeTask, this);
    th.join();
}

void SecurityTask::executeTask()
{
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
    std::cout << "\n Execution of SecurityTask nr: " << taskId_ << "  priority: " << priority_;
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
}

ControlTask::ControlTask(Priority priority) : GeneralTask(priority)
{

}

void ControlTask::execute()
{
    thread th = thread(&ControlTask::executeTask, this);
    th.join();
}

void ControlTask::executeTask()
{
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
    std::cout << "\n Execution of ControlTask nr: " << taskId_ << "  priority: " << priority_;
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
}

ProcessTask::ProcessTask(Priority priority) : GeneralTask(priority)
{
}

void ProcessTask::execute()
{
    thread th = thread(&ProcessTask::executeTask, this);
    th.join();
}

void ProcessTask::executeTask()
{
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
    std::cout << "\n Execution of ProcessTask nr: " << taskId_ << "  priority: " << priority_;
    std::this_thread::sleep_for(std::chrono::milliseconds { 50 });
}

