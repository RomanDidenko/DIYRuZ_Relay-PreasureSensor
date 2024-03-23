#ifndef CONGIG_COMMAND_H
#define CONGIG_COMMAND_H

//#define SERIAL_COMMANDS_DEBUG

#include <SerialCommands.h>

#include "../config/config.h"

class ConfigCommands
{
private:
    static Config *config;

    char serial_command_buffer_[32];
    SerialCommands *serial_commands_;

    SerialCommand** commands;

public:
    ConfigCommands(Stream *stream, Config *_config);
    ~ConfigCommands();
    void Loop();

private:
    static void cmd_help(SerialCommands *sender);
    static void cmd_set_MinReportIntervalInMilliseconds(SerialCommands *sender);
    static void cmd_set_ValueDeltaToForceReport(SerialCommands *sender);
    static void cmd_set_PreassureProbIntervalInMilliseconds(SerialCommands *sender);
    static void cmd_set_ReadingsNumberToTakeAverageOn(SerialCommands *sender);
    static void cmd_state(SerialCommands *sender);
    static void cmd_preassure(SerialCommands *sender);
    static void cmd_help_json(SerialCommands *sender);
    static void cmd_unrecognized(SerialCommands *sender, const char *cmd);
};

#endif /* CONGIG_COMMAND_H */