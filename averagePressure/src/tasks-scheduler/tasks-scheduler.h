#ifndef TASKS_SCHEDULE_PROVIDER_H
#define TASKS_SCHEDULE_PROVIDER_H

#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_PRIORITY
#include <TaskScheduler.h>

//#define PREASSURE_DEBUG
//#define PREASSURE_READING_DEBUG
#include "../preassure-reader/preassure-reader-task.h"

#include "../preassure-reporter/preassure-reporter-task.h"

//#define SERIAL_COMMANDS_DEBUG
#include "../config-commands/config-commands-task.h"

#include "../config-repository/config-saver-task.h"

class TasksScheduler
{
private:
    Scheduler readingScheduler;
    Scheduler reporterScheduler;
    Scheduler configCommandsScheduler;
    Scheduler configSaverScheduler;

    Scheduler *lastScheduler;

public:
    void Setup()
    {
        lastScheduler = &configSaverScheduler;

        configSaverScheduler.setHighPriorityScheduler(&configCommandsScheduler);
        configSaverScheduler.addTask(ConfigSaverTask);

        configCommandsScheduler.setHighPriorityScheduler(&reporterScheduler);
        configCommandsScheduler.addTask(ConfigCommandsTask);

        reporterScheduler.setHighPriorityScheduler(&readingScheduler);
        reporterScheduler.addTask(PreassureReporterTask);

        readingScheduler.addTask(PreassureReaderTask);

        lastScheduler->enableAll(true);
    }

    void Loop()
    {
        lastScheduler->execute();
    }
};

#endif /* TASKS_SCHEDULE_PROVIDER_H */