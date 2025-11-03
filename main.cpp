#include <iostream>

#include "taskscheduler.h"

using namespace std;



int main()
{
    SecurityTask sTask1(1);
    SecurityTask sTask2(2);
    SecurityTask sTask3(4);
    SecurityTask sTask4(1);
    ControlTask cTask1(3);
    ControlTask cTask2(2);
    ControlTask cTask3(1);
    ControlTask cTask4(4);
    ProcessTask pTask1(2);
    ProcessTask pTask2(3);
    ProcessTask pTask3(1);
    ProcessTask pTask4(2);

    sTask1.setOverridingTask(pTask2.getId());
    sTask4.setOverridingTask(cTask4.getId());
    cTask3.setOverridingTask(cTask2.getId());
    cTask3.setOverridingTask(sTask1.getId());

    std::vector<std::unique_ptr<Task>> tasks;
    tasks.push_back(std::make_unique<SecurityTask>(sTask1));
    tasks.push_back(std::make_unique<SecurityTask>(sTask2));
    tasks.push_back(std::make_unique<SecurityTask>(sTask3));
    tasks.push_back(std::make_unique<SecurityTask>(sTask4));
    tasks.push_back(std::make_unique<ControlTask>(cTask1));
    tasks.push_back(std::make_unique<ControlTask>(cTask2));
    tasks.push_back(std::make_unique<ControlTask>(cTask3));
    tasks.push_back(std::make_unique<ControlTask>(cTask4));
    tasks.push_back(std::make_unique<ProcessTask>(pTask1));
    tasks.push_back(std::make_unique<ProcessTask>(pTask2));
    tasks.push_back(std::make_unique<ProcessTask>(pTask3));
    tasks.push_back(std::make_unique<ProcessTask>(pTask4));

    TaskScheduler scheduler;

    for (auto &task : tasks) {
        scheduler.submitTask(std::move(task));
    }

    scheduler.start();

    this_thread::sleep_for(std::chrono::milliseconds { 100 });
    ProcessTask pTask5(2);
    scheduler.submitTask(std::move(std::make_unique<ProcessTask>(pTask5)));

    this_thread::sleep_for(std::chrono::milliseconds { 100 });
    ProcessTask pTask6(1);
    scheduler.submitTask(std::move(std::make_unique<ProcessTask>(pTask6)));

    this_thread::sleep_for(std::chrono::milliseconds { 3000 });

    scheduler.stop();

    std::cout << "\nEnd of process\n\n";
    return 0;
}
