#ifndef SKYMETRIC_APP_CONFIG_H
#define SKYMETRIC_APP_CONFIG_H

#include <Arduino.h>

struct AppConfig
{
  String blynkTemplateId;
  String blynkTemplateName;
  String blynkAuthToken;
  String wifiSsid;
  String wifiPassword;

  bool isValid() const
  {
    return !wifiSsid.isEmpty() && !wifiPassword.isEmpty() && !blynkAuthToken.isEmpty();
  }
};

#endif
