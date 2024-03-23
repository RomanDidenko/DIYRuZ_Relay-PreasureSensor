#ifndef CONFIG_SAVER_TASK_H
#define CONFIG_SAVER_TASK_H

#include <TaskSchedulerDeclarations.h>

#include "../common/subscriber.h"
#include "../config/config.h"
#include "../config-repository/config-repository.h"

extern Config config;

void ConfigSaverTask_Callback()
{
    ConfigRepository::Save(config.GetConfigData());
}

Task ConfigSaverTask(1000, TASK_ONCE, &ConfigSaverTask_Callback);

Subscriber _4([]
              { config.OnChange([]
                                { ConfigSaverTask.restart(); }); });

#endif /* CONFIG_SAVER_TASK_H */