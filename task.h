#ifndef TASK_H
#define TASK_H

#include <memory>
#include <functional>
#include <chrono>

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

protected:
    virtual void executeTask() = 0;

public:
    static TaskId taskIdCounter_;

protected:
    TaskId taskId_;
    Priority priority_;
};

class GeneralTask : public Task
{
public:
    GeneralTask(Priority priority);

    TaskId getId() const override;
    Priority getPriority() const override;
    void execute() override;
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
