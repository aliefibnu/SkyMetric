#include "WebConfigService.h"

#include <WiFi.h>

#include "ConfigStore.h"
#include "Logger.h"

namespace
{
  const char *kApSsid = "SkyMetric-Setup";
  const char *kApPassword = "skymetric123";
}

void WebConfigService::begin(ConfigStore &store, const AppConfig &currentConfig)
{
  store_ = &store;
  currentConfig_ = currentConfig;
  updatedConfig_ = currentConfig;
  configUpdated_ = false;
  restartRequested_ = false;

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(kApSsid, kApPassword);
  active_ = true;

  setupRoutes();
  server_.begin();

  Logger::warn("Config portal started at AP SkyMetric-Setup");
}

void WebConfigService::run()
{
  if (active_)
  {
    server_.handleClient();
  }
}

bool WebConfigService::isActive() const
{
  return active_;
}

bool WebConfigService::isConfigUpdated() const
{
  return configUpdated_;
}

bool WebConfigService::shouldRestart() const
{
  return restartRequested_;
}

AppConfig WebConfigService::takeUpdatedConfig()
{
  configUpdated_ = false;
  return updatedConfig_;
}

void WebConfigService::stop()
{
  if (!active_)
  {
    return;
  }

  server_.stop();
  WiFi.softAPdisconnect(true);
  active_ = false;
}

void WebConfigService::setupRoutes()
{
  server_.on("/", HTTP_GET, [this]()
             { handleRoot(); });
  server_.on("/save", HTTP_POST, [this]()
             { handleSave(); });
  server_.on("/reset", HTTP_POST, [this]()
             { handleReset(); });
}

String WebConfigService::buildPage() const
{
  String page;
  page.reserve(1800);
  page += "<html><head><meta name='viewport' content='width=device-width,initial-scale=1'>";
  page += "<style>body{font-family:Arial,sans-serif;max-width:720px;margin:24px auto;padding:0 16px;}";
  page += "input,button{width:100%;padding:10px;margin:6px 0;box-sizing:border-box;}";
  page += "button{cursor:pointer;}</style></head><body>";
  page += "<h2>SkyMetric Setup</h2>";
  page += "<p>AP SSID: SkyMetric-Setup</p>";
  page += "<form method='post' action='/save'>";
  page += "<label>Blynk Template ID</label><input name='templateId' value='" + currentConfig_.blynkTemplateId + "'>";
  page += "<label>Blynk Template Name</label><input name='templateName' value='" + currentConfig_.blynkTemplateName + "'>";
  page += "<label>Blynk Auth Token</label><input name='authToken' value='" + currentConfig_.blynkAuthToken + "'>";
  page += "<label>WiFi SSID</label><input name='wifiSsid' value='" + currentConfig_.wifiSsid + "'>";
  page += "<label>WiFi Password</label><input name='wifiPassword' type='password' value='" + currentConfig_.wifiPassword + "'>";
  page += "<button type='submit'>Save</button></form>";
  page += "<form method='post' action='/reset'><button type='submit'>Reset to secrets.h default</button></form>";
  page += "</body></html>";
  return page;
}

void WebConfigService::handleRoot()
{
  server_.send(200, "text/html", buildPage());
}

void WebConfigService::handleSave()
{
  updatedConfig_.blynkTemplateId = server_.arg("templateId");
  updatedConfig_.blynkTemplateName = server_.arg("templateName");
  updatedConfig_.blynkAuthToken = server_.arg("authToken");
  updatedConfig_.wifiSsid = server_.arg("wifiSsid");
  updatedConfig_.wifiPassword = server_.arg("wifiPassword");

  if (!updatedConfig_.isValid())
  {
    server_.send(400, "text/plain", "Invalid config: SSID, password, and token must not be empty.");
    Logger::error("Rejected config save because required fields are empty");
    return;
  }

  if (store_)
  {
    store_->save(updatedConfig_);
  }

  configUpdated_ = true;
  restartRequested_ = true;
  currentConfig_ = updatedConfig_;
  server_.send(200, "text/plain", "Saved. Restart device to apply new credentials.");
  Logger::warn("Configuration saved via local web interface");
}

void WebConfigService::handleReset()
{
  if (store_)
  {
    store_->reset();
  }

  configUpdated_ = true;
  restartRequested_ = true;
  server_.send(200, "text/plain", "Reset saved config. Restart device to use secrets.h defaults.");
  Logger::warn("Configuration reset via local web interface");
}
