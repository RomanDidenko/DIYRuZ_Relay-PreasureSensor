#ifndef CONGIG_COMMANDS_TASK_H
#define CONGIG_COMMANDS_TASK_H

#include <TaskSchedulerDeclarations.h>

#include "../config-commands/config-commands.h"

extern Config config;

ConfigCommands configCommands(&Serial, &config);

void ConfigCommandsTask_Callback()
{
    configCommands.Loop();
}

Task ConfigCommandsTask(300, TASK_FOREVER, &ConfigCommandsTask_Callback);

#endif /* CONGIG_COMMANDS_TASK_H */