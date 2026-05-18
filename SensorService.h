#ifndef SKYMETRIC_SENSOR_SERVICE_H
#define SKYMETRIC_SENSOR_SERVICE_H

#include <Arduino.h>

class DHT;

struct SensorData
{
  int temperatureC = -1;
  int humidityPercent = -1;
  int guvaRaw = 0;
  int guvaCentivolt = 0;
};

class SensorService
{
public:
  SensorService();

  void begin();
  bool shouldRead(unsigned long nowMs) const;
  bool read(SensorData &outData);
  ~SensorService();

private:
  DHT *dht_;
  unsigned long lastReadMs_ = 0;
};

#endif
