#include "RunningAverage.h"

#define MinReportTimeInMilliseconds 1
#define ValueDeltaToForceReport 1

#define AverageBufferSize 15
#define ProbeDelayInMilliseconds 10

RunningAverage avePressure(AverageBufferSize);

int samples = 0;
int value;

int lastValue = -100;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);

  avePressure.clear();

   while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
}

void loop() {
  value = analogRead(A0);
  
  avePressure.addValue(value);
  
  value = round(avePressure.getAverage());

  if (millis() - lastTime > MinReportTimeInMilliseconds
      || abs(value - lastValue) > ValueDeltaToForceReport)
  {
    lastTime = millis();
    lastValue = value;
    
    Serial.println(value);
  }

  // delay 10ms to let the ADC recover:
  delay(ProbeDelayInMilliseconds);
}
