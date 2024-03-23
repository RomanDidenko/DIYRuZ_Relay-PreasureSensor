#ifndef PREASSURE_REPORTER_TASK_H
#define PREASSURE_REPORTER_TASK_H

#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>

#include "../common/common.h"
#include "../common/subscriber.h"
#include "../config/config.h"
#include "../preassure-reporter/preassure-reporter.h"

#ifdef PREASSURE_DEBUG
#define Println_DEBUG(...) Println(Serial, __VA_ARGS__)
#else 
#define Println_DEBUG(...) DONOTHING
#endif

extern Config config;

void PreassureReporterTask_Callback();
Task PreassureReporterTask(config.GetMinReportIntervalInMilliseconds(), TASK_FOREVER, &PreassureReporterTask_Callback);

void StateReporterTask_Callback();
Task StateReporterTask(500, TASK_ONCE, &StateReporterTask_Callback);

Subscriber _3([]
              { config.OnChange([]
                                { PreassureReporterTask.setInterval(config.GetMinReportIntervalInMilliseconds()); }); });

PreassureReporter reporter(&config, &PreassureReporterTask);

void PreassureReporterTask_Callback()
{
    Println_DEBUG("--> Reporting Preassure");

    reporter.ReportCurrentValue(&Serial);
}

void StateReporterTask_Callback()
{
    Println_DEBUG("--> Reporting State");

    reporter.ReportState(&Serial, &config);
}

#endif /* PREASSURE_REPORTER_TASK_H */