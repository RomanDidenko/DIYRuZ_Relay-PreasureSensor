#include <ESP8266WiFi.h>

#include "src/config/config.h"
#include "src/config-repository/config-repository.h"

Config config;

// tasks scheduler should be configured after config instantiation.
#include "src/tasks-scheduler/tasks-scheduler.h"

TasksScheduler tasksScheduler;

int main()
{
  init();
  setup();

  for (;;)
  {
    loop();

    if (serialEventRun)
    {
      serialEventRun();
    }
  }
}

void setup()
{
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();

  Serial.begin(9600);
  //Serial1.begin(9600);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  ConfigData data = ConfigRepository::Load();
  config.UpdateFrom(&data);

  tasksScheduler.Setup();

  //Println(&Serial, "Setup completed");
}

void loop()
{
  tasksScheduler.Loop();
}