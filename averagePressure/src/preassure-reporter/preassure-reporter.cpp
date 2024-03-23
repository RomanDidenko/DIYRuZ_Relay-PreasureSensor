#include <Arduino.h>

#include "../common/common.h"
#include "preassure-reporter.h"

#ifdef PREASSURE_DEBUG
#define Println_DEBUG(...) Println(Serial, __VA_ARGS__)
#else 
#define Println_DEBUG(...) DONOTHING
#endif

PreassureReporter::PreassureReporter(Config *config, Task *reporterTask)
    : config(config),
      reporterTask(reporterTask)
{
}

int PreassureReporter::currentValue = PreassureReporter::InitialValue;
int PreassureReporter::lastReportedValue = PreassureReporter::InitialValue;

void PreassureReporter::ProcessCurrentValue(int value)
{
    currentValue = value;

    if (abs(currentValue - lastReportedValue) > config->GetValueDeltaToForceReport())
    {
        Println_DEBUG("--> Force Preassure Reporting");
        reporterTask->forceNextIteration();
    }
}

void PreassureReporter::ReportCurrentValue(Stream* serial)
{
    if (currentValue == InitialValue)
    {
        return;
    }

    lastReportedValue = currentValue;

    Println(serial, currentValue);
}

void PreassureReporter::ReportState(Stream* serial, Config* config)
{   
    Print(serial,
        "{\"delta\":", config->GetValueDeltaToForceReport(),
        ", \"report_interval\":", config->GetMinReportIntervalInMilliseconds(),
        ", \"prob_interval\":", config->GetPreassureProbIntervalInMilliseconds(),
        ", \"average_on_items\":", config->GetReadingsNumberToTakeAverageOn(),
        ", \"uptime\":", millis() / 1000,
        ", \"preassure_adc\":");
        
        if(currentValue == InitialValue){
            Print(serial, "null");
        }
        else
        {
            Print(serial, currentValue);
        }

        Println(serial, "}");
}