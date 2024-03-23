#ifndef CONGIG_REPOSITORY_H
#define CONGIG_REPOSITORY_H

#include <Arduino.h>

#include "../config/config-data.h"

#include <EEPROM_Rotate.h>

class ConfigRepository
{
private:
    static EEPROM_Rotate eeprom;

    static bool isInitialized;

    static void SetupIfNot()
    {
        if (isInitialized)
        {
            return;
        }

        isInitialized = true;

        eeprom.offset(0xFF0);
        eeprom.begin(4096);
    }

public:
    static ConfigData Load()
    {
        SetupIfNot();

        ConfigData defaultConfig;

        ConfigData data;
        data = eeprom.get(0, data);

        if (strncmp(defaultConfig.version, data.version, sizeof(defaultConfig.version) / sizeof(defaultConfig.version[0]) != 0))
        {
            return defaultConfig;
        }

        return data;
    }

    static void Save(ConfigData data)
    {
        SetupIfNot();

        eeprom.put(0, data);
        eeprom.commit();
    }
};

EEPROM_Rotate ConfigRepository::eeprom;
bool ConfigRepository::isInitialized = false;

#endif /* CONGIG_REPOSITORY_H */