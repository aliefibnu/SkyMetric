#include "BlynkService.h"
#include "Config.h"
#include "LedService.h"
#include "Logger.h"
#include "SensorService.h"

LedService ledService;
SensorService sensorService;
BlynkService blynkService;

void setup()
{
  Logger::begin();
  ledService.begin();
  sensorService.begin();
  blynkService.begin(ledService);
}

void loop()
{
  const unsigned long nowMs = millis();

  blynkService.run(ledService);
  ledService.update(nowMs);

  if (sensorService.shouldRead(nowMs))
  {
    SensorData data;
    const bool hasValidDht = sensorService.read(data);

    if (hasValidDht)
    {
      blynkService.publish(
          ledService.toStatusJson(),
          data.temperatureC,
          data.humidityPercent,
          data.guvaRaw,
          data.guvaCentivolt);
    }
    else
    {
      blynkService.publish(
          ledService.toStatusJson(),
          -1,
          -1,
          data.guvaRaw,
          data.guvaCentivolt);
    }
  }
}