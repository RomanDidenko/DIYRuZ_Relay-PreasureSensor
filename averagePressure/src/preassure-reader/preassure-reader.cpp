#include "preassure-reader.h"

PreassureReader::PreassureReader(int readingsNumberToTakeAverageOn, const uint8_t port)
    : readingsNumberToTakeAverageOn(readingsNumberToTakeAverageOn),
      port(port)
{
    SetReadingsNumberToTakeAverageOn(readingsNumberToTakeAverageOn);
}

PreassureReader::~PreassureReader()
{
    delete avePressure;
}

void PreassureReader::SetReadingsNumberToTakeAverageOn(int number)
{
    if (number <= 0)
    {
        number = 1;
    }

    if (readingsNumberToTakeAverageOn == number && avePressure != nullptr)
    {
        return;
    }

    readingsNumberToTakeAverageOn = number;

    if (avePressure != nullptr)
    {
        delete avePressure;
    }

    avePressure = new RunningAverage(readingsNumberToTakeAverageOn);
    avePressure->clear();
}

void PreassureReader::ReadPreassure()
{
    int value = analogRead(port);

    avePressure->addValue(value);
}

int PreassureReader::GetAveragePressure()
{
    return round(avePressure->getAverage());
}