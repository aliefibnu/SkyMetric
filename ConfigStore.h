#ifndef SKYMETRIC_CONFIG_STORE_H
#define SKYMETRIC_CONFIG_STORE_H

#include <Arduino.h>

#include "AppConfig.h"

class ConfigStore
{
public:
  void begin();
  AppConfig load(const AppConfig &defaults);
  void save(const AppConfig &config);
  void reset();

private:
  static constexpr const char *kNamespace = "skymetric";
  static constexpr const char *kHasConfigKey = "hasConfig";
  static constexpr const char *kTemplateIdKey = "templateId";
  static constexpr const char *kTemplateNameKey = "templateName";
  static constexpr const char *kAuthTokenKey = "authToken";
  static constexpr const char *kWifiSsidKey = "wifiSsid";
  static constexpr const char *kWifiPasswordKey = "wifiPassword";
};

#endif
