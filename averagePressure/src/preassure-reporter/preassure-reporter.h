#ifndef PREASSURE_REPORTER_H
#define PREASSURE_REPORTER_H

#include <TaskSchedulerDeclarations.h>

#include "../config/config.h"

class PreassureReporter
{
private:
    Config* config;
    Task* reporterTask;
    static int currentValue;
    static int lastReportedValue;
    const static int InitialValue = -111115;

public:
    PreassureReporter(Config* config, Task* reportingTask);
    void ProcessCurrentValue(int value);
    static void ReportCurrentValue(Stream* serial);
    static void ReportState(Stream* serial, Config* config);
};

#endif /* PREASSURE_REPORTER_H */