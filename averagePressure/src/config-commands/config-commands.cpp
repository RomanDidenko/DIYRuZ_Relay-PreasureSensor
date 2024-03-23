//#define SERIAL_COMMANDS_DEBUG
#include <Arduino.h>
#include <SerialCommands.h>

#include "../config/config.h"
#include "config-commands.h"

#include "../preassure-reporter/preassure-reporter.h"

Config *ConfigCommands::config = NULL;

ConfigCommands::ConfigCommands(Stream *serial, Config *_config)
{
    config = _config;

    serial_commands_ = new SerialCommands(serial, serial_command_buffer_, sizeof(serial_command_buffer_), ";", " ");

    const byte commandsNumber = 12;
    commands = new SerialCommand *[commandsNumber]
    {
        new SerialCommand("set-value-delta-to-force-delta", cmd_set_ValueDeltaToForceReport),
            new SerialCommand("d", cmd_set_ValueDeltaToForceReport),

            new SerialCommand("set-min-report-time-in-milliseconds", cmd_set_MinReportIntervalInMilliseconds),
            new SerialCommand("ri", cmd_set_MinReportIntervalInMilliseconds),

            new SerialCommand("set-preassure-prob-interval-in-milliseconds", cmd_set_PreassureProbIntervalInMilliseconds),
            new SerialCommand("pi", cmd_set_PreassureProbIntervalInMilliseconds),

            new SerialCommand("set-readings-number-to-take-average-on", cmd_set_ReadingsNumberToTakeAverageOn),
            new SerialCommand("rn", cmd_set_ReadingsNumberToTakeAverageOn),

            new SerialCommand("help", cmd_help),
            new SerialCommand("help-json", cmd_help_json),

            new SerialCommand("state", cmd_state),

            new SerialCommand("preassure", cmd_preassure),
    };

    serial_commands_->SetDefaultHandler(cmd_unrecognized);

    for (byte i = 0; i < commandsNumber; i++)
    {
        serial_commands_->AddCommand(commands[i]);
    }
}

ConfigCommands::~ConfigCommands()
{
    delete serial_commands_;
    delete[] commands;
}

void ConfigCommands::Loop()
{
    //DebugCommands("--> ConfigCommands::Loop");
    serial_commands_->ReadSerial();
}

void ConfigCommands::cmd_unrecognized(SerialCommands *sender, const char *cmd)
{
#ifdef SERIAL_COMMANDS_DEBUG
    Stream *serial = sender->GetSerial();

    Println(serial, "Unrecognized command [", cmd, "]");
    cmd_help_json(sender);
#endif
}

void ConfigCommands::cmd_set_MinReportIntervalInMilliseconds(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();

    // Note: Every call to Next moves the pointer to next parameter
    char *minReportTimeText = sender->Next();
    if (minReportTimeText == NULL)
    {
        Println(serial, "ERROR: no min report time has been provided");
        return;
    }

    config->SetMinReportIntervalInMilliseconds(atoi(minReportTimeText));

    cmd_state(sender);
}

void ConfigCommands::cmd_set_ValueDeltaToForceReport(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();

    char *valueDeltaText = sender->Next();
    if (valueDeltaText == NULL)
    {
        Println(serial, "ERROR: no value delta that should force report has been provided");
        return;
    }

    ConfigCommands::config->SetValueDeltaToForceReport(atoi(valueDeltaText));

    cmd_state(sender);
}

void ConfigCommands::cmd_set_PreassureProbIntervalInMilliseconds(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();

    char *valueDeltaText = sender->Next();
    if (valueDeltaText == NULL)
    {
        Println(serial, "ERROR: no value detlate to force report has been provided");
        return;
    }

    ConfigCommands::config->SetPreassureProbIntervalInMilliseconds(atoi(valueDeltaText));

    cmd_state(sender);
}

void ConfigCommands::cmd_set_ReadingsNumberToTakeAverageOn(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();

    char *valueDeltaText = sender->Next();
    if (valueDeltaText == NULL)
    {
        Println(serial, "ERROR: no value detlate to force report has been provided");
        return;
    }

    ConfigCommands::config->SetReadingsNumberToTakeAverageOn(atoi(valueDeltaText));

    cmd_state(sender);
}

void ConfigCommands::cmd_help(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();

    Println(serial, "Available commands:");
    Println(serial, "set-value-delta-to-force-delta (d): ", ConfigCommands::config->GetValueDeltaToForceReport());
    Println(serial, "set-min-report-interval-in-milliseconds (ri): ", ConfigCommands::config->GetMinReportIntervalInMilliseconds());
    Println(serial, "set-preassure-prob-interval-in-milliseconds (pi): ", ConfigCommands::config->GetPreassureProbIntervalInMilliseconds());
    Println(serial, "set-readings-number-to-take-average-on (rn): ", ConfigCommands::config->GetReadingsNumberToTakeAverageOn());
}

void ConfigCommands::cmd_state(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();
    
    PreassureReporter::ReportState(serial, ConfigCommands::config);
}

void ConfigCommands::cmd_preassure(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();
    
    PreassureReporter::ReportCurrentValue(serial);
}

void ConfigCommands::cmd_help_json(SerialCommands *sender)
{
    Stream *serial = sender->GetSerial();

    Println(serial,
            "{\"set-value-delta-to-force-delta (d)\": ", ConfigCommands::config->GetValueDeltaToForceReport(),
            ", \"set-min-report-interval-in-milliseconds (ri)\": ", ConfigCommands::config->GetMinReportIntervalInMilliseconds(),
            ", \"set-preassure-prob-interval-in-milliseconds (pi)\": ", ConfigCommands::config->GetPreassureProbIntervalInMilliseconds(),
            ", \"set-readings-number-to-take-average-on (rn)\": ", ConfigCommands::config->GetReadingsNumberToTakeAverageOn(),
            "}");
}