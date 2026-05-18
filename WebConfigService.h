#ifndef SKYMETRIC_WEB_CONFIG_SERVICE_H
#define SKYMETRIC_WEB_CONFIG_SERVICE_H

#include <Arduino.h>
#include <WebServer.h>

#include "AppConfig.h"

class ConfigStore;

class WebConfigService
{
public:
  void begin(ConfigStore &store, const AppConfig &currentConfig);
  void run();
  bool isActive() const;
  bool isConfigUpdated() const;
  bool shouldRestart() const;
  AppConfig takeUpdatedConfig();
  void stop();

private:
  WebServer server_{80};
  ConfigStore *store_ = nullptr;
  AppConfig currentConfig_;
  AppConfig updatedConfig_;
  bool active_ = false;
  bool configUpdated_ = false;
  bool restartRequested_ = false;

  void setupRoutes();
  String buildPage() const;
  void handleRoot();
  void handleSave();
  void handleReset();
};

#endif
