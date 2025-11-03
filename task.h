#ifndef TASK_H
#define TASK_H

#include <memory>
#include <functional>
#include <chrono>
#include <unordered_set>

class Task
{
public:
    using TaskId = uint64_t;
    using Priority = int;

    Task(Priority priority);

    virtual ~Task() = default;
    virtual void execute() = 0;
    virtual TaskId getId() const = 0;
    virtual Priority getPriority() const = 0;
    virtual void setOverridingTask(const TaskId taskId) = 0;
    virtual void removeDependentTask(const TaskId taskId) = 0;
    virtual bool isIndependent() = 0;

protected:
    virtual void executeTask() = 0;

public:
    static TaskId taskIdCounter_;

protected:
    TaskId taskId_;
    Priority priority_;
    std::unordered_set<TaskId> dependencies_;
};

class GeneralTask : public Task
{
public:
    GeneralTask(Priority priority);

    void execute() override;
    TaskId getId() const override;
    Priority getPriority() const override;
    void setOverridingTask(const TaskId taskId) override;
    void removeDependentTask(const TaskId taskId) override;
    bool isIndependent() override;
};

class SecurityTask : public GeneralTask
{
public:
    SecurityTask(Priority priority);

private:
    void executeTask() override;
};

class ControlTask : public GeneralTask
{
public:
    ControlTask(Priority priority);

private:
    void executeTask() override;
};

class ProcessTask : public GeneralTask
{
public:
    ProcessTask(Priority priority);

private:
    void executeTask() override;
};


#endif // TASK_H
