#include "SensorService.h"

#include <DHT.h>
#include <math.h>

#include "Config.h"
#include "Logger.h"

namespace
{
  constexpr uint8_t kDhtType = DHT22;
}

SensorService::SensorService()
{
  dht_ = new DHT(Config::PIN_DHT, kDhtType);
}

void SensorService::begin()
{
  dht_->begin();
  lastReadMs_ = 0;
}

SensorService::~SensorService()
{
  if (dht_)
  {
    delete dht_;
  }
}

bool SensorService::shouldRead(unsigned long nowMs) const
{
  return (nowMs - lastReadMs_) >= Config::SENSOR_READ_INTERVAL_MS;
}

bool SensorService::read(SensorData &outData)
{
  const unsigned long nowMs = millis();
  lastReadMs_ = nowMs;

  const float humidity = dht_->readHumidity();
  const float temperatureC = dht_->readTemperature();

  outData.guvaRaw = analogRead(Config::PIN_GUVA);
  const float guvaVolt =
      (static_cast<float>(outData.guvaRaw) / Config::ADC_MAX_VALUE) *
      Config::ADC_REFERENCE_VOLT;
  outData.guvaCentivolt = static_cast<int>(roundf(guvaVolt * 100.0f));

  if (isnan(humidity) || isnan(temperatureC))
  {
    Logger::warn("DHT22 read failed, publishing -1 for temperature and humidity");
    outData.temperatureC = -1;
    outData.humidityPercent = -1;
    return false;
  }

  outData.temperatureC = static_cast<int>(roundf(temperatureC));
  outData.humidityPercent = static_cast<int>(roundf(humidity));
  return true;
}
