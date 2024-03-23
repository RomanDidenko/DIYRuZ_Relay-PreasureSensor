#ifndef CONGIG_H
#define CONGIG_H

#include "config-data.h"
#include "../common/common.h"

#ifdef SERIAL_COMMANDS_DEBUG
#define DebugCommands(text) Println(text);
#else
#define DebugCommands(text) DONOTHING
#endif

typedef void (*ConfigChangedCallback)();

class Config
{
private:
    ConfigChangedCallback callbacks[6];
    int minReportIntervalInMilliseconds = 15000;
    int valueDeltaToForceReport = 1;
    int preassureProbIntervalInMilliseconds = 10;
    int readingsNumberToTakeAverageOn = 20;

public:
    Config();
    int GetMinReportIntervalInMilliseconds();
    void SetMinReportIntervalInMilliseconds(int value);

    int GetValueDeltaToForceReport();
    void SetValueDeltaToForceReport(int value);

    int GetPreassureProbIntervalInMilliseconds();
    void SetPreassureProbIntervalInMilliseconds(int value);

    int GetReadingsNumberToTakeAverageOn();
    void SetReadingsNumberToTakeAverageOn(int value);

    bool OnChange(ConfigChangedCallback callback);

    void UpdateFrom(ConfigData* data);
    ConfigData GetConfigData();

private:
    void OnConfigChanged();
};

#endif /* CONGIG_H */