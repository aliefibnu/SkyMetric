#include "ConfigStore.h"

#include <Preferences.h>

namespace
{
  Preferences preferences;
}

void ConfigStore::begin()
{
  preferences.begin(kNamespace, false);
}

AppConfig ConfigStore::load(const AppConfig &defaults)
{
  AppConfig config = defaults;

  if (!preferences.getBool(kHasConfigKey, false))
  {
    return config;
  }

  const String templateId = preferences.getString(kTemplateIdKey, defaults.blynkTemplateId);
  const String templateName = preferences.getString(kTemplateNameKey, defaults.blynkTemplateName);
  const String authToken = preferences.getString(kAuthTokenKey, defaults.blynkAuthToken);
  const String wifiSsid = preferences.getString(kWifiSsidKey, defaults.wifiSsid);
  const String wifiPassword = preferences.getString(kWifiPasswordKey, defaults.wifiPassword);

  if (!authToken.isEmpty() && !wifiSsid.isEmpty() && !wifiPassword.isEmpty())
  {
    config.blynkTemplateId = templateId;
    config.blynkTemplateName = templateName;
    config.blynkAuthToken = authToken;
    config.wifiSsid = wifiSsid;
    config.wifiPassword = wifiPassword;
  }

  return config;
}

void ConfigStore::save(const AppConfig &config)
{
  preferences.putBool(kHasConfigKey, true);
  preferences.putString(kTemplateIdKey, config.blynkTemplateId);
  preferences.putString(kTemplateNameKey, config.blynkTemplateName);
  preferences.putString(kAuthTokenKey, config.blynkAuthToken);
  preferences.putString(kWifiSsidKey, config.wifiSsid);
  preferences.putString(kWifiPasswordKey, config.wifiPassword);
}

void ConfigStore::reset()
{
  preferences.clear();
}
