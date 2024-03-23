#ifndef PREASSURE_READER_TASK_H
#define PREASSURE_READER_TASK_H

#include <TaskSchedulerDeclarations.h>

#include "../common/common.h"
#include "../common/subscriber.h"
#include "../config/config.h"
#include "preassure-reader.h"

#include "../preassure-reporter/preassure-reporter.h"

#ifdef PREASSURE_READING_DEBUG
#define Println_DEBUG(...) Println(Serial, __VA_ARGS__)
#else 
#define Println_DEBUG(...) DONOTHING
#endif

extern Config config;
extern PreassureReporter reporter;

PreassureReader reader(config.GetReadingsNumberToTakeAverageOn(), A0);
Subscriber _2([]
              { config.OnChange([]
                                { reader.SetReadingsNumberToTakeAverageOn(config.GetReadingsNumberToTakeAverageOn()); }); });

extern Task PreassureReporterTask;

int currentPreassure = 0;

void PreassureReaderTask_Callback()
{
    Println_DEBUG("--> Reading Preassure");

    reader.ReadPreassure();

    int value = reader.GetAveragePressure();

    Println_DEBUG("--> Read value: ", value);

    reporter.ProcessCurrentValue(value);
}

Task PreassureReaderTask(config.GetPreassureProbIntervalInMilliseconds(), TASK_FOREVER, &PreassureReaderTask_Callback);
Subscriber _1([]
              { config.OnChange([]
                                { PreassureReaderTask.setInterval(config.GetPreassureProbIntervalInMilliseconds()); }); });

#endif /* PREASSURE_READER_TASK_H */