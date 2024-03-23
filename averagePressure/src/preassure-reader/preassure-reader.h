#ifndef PREASSURE_READER_H
#define PREASSURE_READER_H

#include <RunningAverage.h>

#include "../config/config.h"

class PreassureReader
{
private:
    const uint8_t port;
    RunningAverage* avePressure;
    int readingsNumberToTakeAverageOn;
    
public:
    PreassureReader(int readingsNumberToTakeAverageOn, const uint8_t port);
    ~PreassureReader();
    void ReadPreassure();
    void SetReadingsNumberToTakeAverageOn(int readingsNumberToTakeAverageOn);
    int GetAveragePressure();
};

#endif /* PREASSURE_READER_H */