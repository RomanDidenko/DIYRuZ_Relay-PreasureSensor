#include "config.h"

Config::Config() : callbacks({nullptr}) {}

int Config::GetMinReportIntervalInMilliseconds()
{
    return minReportIntervalInMilliseconds;
};
void Config::SetMinReportIntervalInMilliseconds(int value)
{
    if (minReportIntervalInMilliseconds == value)
    {
        return;
    }

    minReportIntervalInMilliseconds = value;
    OnConfigChanged();
}

int Config::GetValueDeltaToForceReport()
{
    return valueDeltaToForceReport;
};
void Config::SetValueDeltaToForceReport(int value)
{
    if (valueDeltaToForceReport == value)
    {
        return;
    }

    valueDeltaToForceReport = value;
    OnConfigChanged();
}

int Config::GetPreassureProbIntervalInMilliseconds()
{
    return preassureProbIntervalInMilliseconds;
};
void Config::SetPreassureProbIntervalInMilliseconds(int value)
{
    if (preassureProbIntervalInMilliseconds == value)
    {
        return;
    }

    preassureProbIntervalInMilliseconds = value;
    OnConfigChanged();
}

int Config::GetReadingsNumberToTakeAverageOn()
{
    return readingsNumberToTakeAverageOn;
}

void Config::SetReadingsNumberToTakeAverageOn(int value)
{
    if (readingsNumberToTakeAverageOn == value)
    {
        return;
    }

    readingsNumberToTakeAverageOn = value;
    OnConfigChanged();
}

bool Config::OnChange(ConfigChangedCallback callback)
{
    for (int i = 0; i < sizeof(callbacks) / sizeof(callbacks[0]); i++)
    {
        if (callbacks[i] == nullptr)
        {
            callbacks[i] = callback;
            return true;
        }
    }

    return true;
}

void Config::OnConfigChanged()
{
    for (int i = 0; i < sizeof(callbacks) / sizeof(callbacks[0]); i++)
    {
        if (callbacks[i] == nullptr)
        {
            return;
        }

        callbacks[i]();
    }
}

void Config::UpdateFrom(ConfigData *data)
{
    minReportIntervalInMilliseconds = data->minReportIntervalInMilliseconds;
    valueDeltaToForceReport = data->valueDeltaToForceReport;
    preassureProbIntervalInMilliseconds = data->preassureProbIntervalInMilliseconds;
    readingsNumberToTakeAverageOn = data->readingsNumberToTakeAverageOn;
}

ConfigData Config::GetConfigData()
{
    ConfigData data;

    data.minReportIntervalInMilliseconds = minReportIntervalInMilliseconds;
    data.valueDeltaToForceReport = valueDeltaToForceReport;
    data.preassureProbIntervalInMilliseconds = preassureProbIntervalInMilliseconds;
    data.readingsNumberToTakeAverageOn = readingsNumberToTakeAverageOn;

    return data;
}