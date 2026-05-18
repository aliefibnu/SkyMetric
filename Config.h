#ifndef SKYMETRIC_CONFIG_H
#define SKYMETRIC_CONFIG_H

#include <Arduino.h>

namespace Config
{
  constexpr uint8_t PIN_LED_ONBOARD = 2;
  constexpr uint8_t PIN_LED_GREEN = 25;
  constexpr uint8_t PIN_LED_RED = 26;

  constexpr uint8_t PIN_DHT = 4;
  constexpr uint8_t PIN_GUVA = 34;

  constexpr float ADC_REFERENCE_VOLT = 3.3f;
  constexpr float ADC_MAX_VALUE = 4095.0f;

  constexpr unsigned long LED_TOGGLE_INTERVAL_MS = 1000;
  constexpr unsigned long SENSOR_READ_INTERVAL_MS = 2000;
} // namespace Config

#endif
