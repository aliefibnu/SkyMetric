#ifndef SKYMETRIC_BLYNK_SERVICE_H
#define SKYMETRIC_BLYNK_SERVICE_H

#include <Arduino.h>

class LedService;
struct AppConfig;

class BlynkService
{
public:
  bool begin(LedService &ledService, const AppConfig &config);
  void run(LedService &ledService);
  void publish(
      const String &ledStatusJson,
      int temperatureC,
      int humidityPercent,
      int guvaRaw,
      int guvaCentivolt);
};

#endif
