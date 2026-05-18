#include "BlynkService.h"

#include "AppConfig.h"
#include "LedService.h"
#include "Logger.h"
#include "secrets.h"

#include <WiFi.h>

#include <BlynkSimpleEsp32.h>

bool BlynkService::begin(LedService &ledService, const AppConfig &config)
{
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  Logger::warn("Starting WiFi connection");
  ledService.setGreenConnecting();
  ledService.startBlockingProcess();

  WiFi.begin(config.wifiSsid.c_str(), config.wifiPassword.c_str());
  const unsigned long wifiStartMs = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - wifiStartMs) < 15000)
  {
    ledService.update(millis());
    delay(50);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    ledService.stopBlockingProcess();
    ledService.setGreenFatal();
    Logger::error("WiFi connection timeout, config portal remains available");
    return false;
  }

  ledService.stopBlockingProcess();
  ledService.setGreenConnected();

  Logger::warn("WiFi connected, starting Blynk connection");

  Blynk.config(config.blynkAuthToken.c_str());
  if (!Blynk.connect(5000))
  {
    Logger::error("Blynk initial connection failed");
    ledService.setRedMiniError();
  }
  else
  {
    ledService.setRedDefault();
  }

  return true;
}

void BlynkService::run(LedService &ledService)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    static unsigned long lastReconnectAttemptMs = 0;
    if ((millis() - lastReconnectAttemptMs) < 10000)
    {
      return;
    }

    lastReconnectAttemptMs = millis();
    Logger::warn("WiFi disconnected, retrying connection");
    ledService.setGreenConnecting();
    ledService.startBlockingProcess();
    WiFi.reconnect();

    const unsigned long reconnectStartMs = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - reconnectStartMs) < 3000)
    {
      ledService.update(millis());
      delay(50);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      ledService.stopBlockingProcess();
      ledService.setGreenConnected();
      Logger::warn("WiFi reconnected");
    }
    else
    {
      ledService.stopBlockingProcess();
      ledService.setRedMiniError();
      Logger::warn("WiFi reconnect attempt failed");
      return;
    }
  }

  if (!Blynk.connected())
  {
    Logger::warn("Blynk disconnected, retrying connection");
    if (!Blynk.connect(1000))
    {
      Logger::error("Blynk reconnect failed");
      ledService.setRedMiniError();
    }
    else
    {
      ledService.setRedDefault();
    }
  }

  Blynk.run();
}

void BlynkService::publish(
    const String &ledStatusJson,
    int temperatureC,
    int humidityPercent,
    int guvaRaw,
    int guvaCentivolt)
{
  Blynk.virtualWrite(V1, ledStatusJson);
  Blynk.virtualWrite(V2, temperatureC);
  Blynk.virtualWrite(V3, humidityPercent);
  Blynk.virtualWrite(V4, guvaRaw);
  Blynk.virtualWrite(V5, guvaCentivolt);
}
