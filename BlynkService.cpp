#include "BlynkService.h"

#include "LedService.h"
#include "Logger.h"
#include "secrets.h"

#include <WiFi.h>

#include <BlynkSimpleEsp32.h>

void BlynkService::begin(LedService &ledService)
{
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  Logger::warn("Starting WiFi connection");
  ledService.setGreenConnecting();
  ledService.startBlockingProcess();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    ledService.update(millis());
    delay(50);
  }

  ledService.stopBlockingProcess();
  ledService.setGreenConnected();

  Logger::warn("WiFi connected, starting Blynk connection");

  Blynk.config(BLYNK_AUTH_TOKEN);
  if (!Blynk.connect(5000))
  {
    Logger::error("Blynk initial connection failed");
  }
}

void BlynkService::run(LedService &ledService)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Logger::warn("WiFi disconnected, retrying connection");
    ledService.setGreenConnecting();
    ledService.startBlockingProcess();
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
      ledService.update(millis());
      delay(50);
    }

    ledService.stopBlockingProcess();
    ledService.setGreenConnected();

    Logger::warn("WiFi reconnected");
  }

  if (!Blynk.connected())
  {
    Logger::warn("Blynk disconnected, retrying connection");
    if (!Blynk.connect(1000))
    {
      Logger::error("Blynk reconnect failed");
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
