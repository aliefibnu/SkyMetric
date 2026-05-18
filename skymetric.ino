#include "BlynkService.h"
#include "AppConfig.h"
#include "Config.h"
#include "ConfigStore.h"
#include "LedService.h"
#include "Logger.h"
#include "WebConfigService.h"
#include "SensorService.h"
#include "secrets.h"

LedService ledService;
SensorService sensorService;
BlynkService blynkService;
ConfigStore configStore;
WebConfigService webConfigService;
AppConfig defaultConfig;
AppConfig activeConfig;

void loadDefaultConfig()
{
  defaultConfig.blynkTemplateId = BLYNK_TEMPLATE_ID;
  defaultConfig.blynkTemplateName = BLYNK_TEMPLATE_NAME;
  defaultConfig.blynkAuthToken = BLYNK_AUTH_TOKEN;
  defaultConfig.wifiSsid = WIFI_SSID;
  defaultConfig.wifiPassword = WIFI_PASSWORD;
}

void setup()
{
  Logger::begin();
  configStore.begin();
  loadDefaultConfig();
  activeConfig = configStore.load(defaultConfig);

  ledService.begin();
  sensorService.begin();
  webConfigService.begin(configStore, activeConfig);

  if (blynkService.begin(ledService, activeConfig))
  {
    Logger::warn("Telemetry started with active configuration");
  }
}

void loop()
{
  const unsigned long nowMs = millis();

  webConfigService.run();
  blynkService.run(ledService);
  ledService.update(nowMs);

  if (webConfigService.shouldRestart())
  {
    Logger::warn("Restarting to apply saved configuration");
    delay(500);
    ESP.restart();
  }

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