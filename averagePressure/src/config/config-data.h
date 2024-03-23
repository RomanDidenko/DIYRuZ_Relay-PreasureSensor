#ifndef CONGIG_DATA_H
#define CONGIG_DATA_H

struct ConfigData {
    int minReportIntervalInMilliseconds = 15000;
    int valueDeltaToForceReport = 1;
    int preassureProbIntervalInMilliseconds = 10;
    int readingsNumberToTakeAverageOn = 20;
    char version[4] = "v01";
};

#endif /* CONGIG_DATA_H */