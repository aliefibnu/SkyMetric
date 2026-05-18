#ifndef SKYMETRIC_BLYNK_SERVICE_H
#define SKYMETRIC_BLYNK_SERVICE_H

#include <Arduino.h>

class LedService;

class BlynkService
{
public:
  void begin(LedService &ledService);
  void run(LedService &ledService);
  void publish(
      const String &ledStatusJson,
      int temperatureC,
      int humidityPercent,
      int guvaRaw,
      int guvaCentivolt);
};

#endif
