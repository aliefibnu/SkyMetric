#include "LedService.h"

#include "Config.h"

void LedService::begin()
{
  pinMode(Config::PIN_LED_ONBOARD, OUTPUT);
  pinMode(Config::PIN_LED_GREEN, OUTPUT);
  pinMode(Config::PIN_LED_RED, OUTPUT);

  setChannel(onboard_, Mode::Off, false);
  setChannel(green_, Mode::Off, false);
  setChannel(red_, Mode::Off, false);
  applyState();
}

void LedService::update(unsigned long nowMs)
{
  updateChannel(onboard_, nowMs);
  updateChannel(green_, nowMs);
  updateChannel(red_, nowMs);
  applyState();
}

String LedService::toStatusJson() const
{
  return "{\"onboard\":" + String(channelOutput(onboard_) ? 1 : 0) +
         ",\"green\":" + String(channelOutput(green_) ? 1 : 0) +
         ",\"red\":" + String(channelOutput(red_) ? 1 : 0) + "}";
}

void LedService::applyState()
{
  digitalWrite(Config::PIN_LED_ONBOARD, channelOutput(onboard_) ? HIGH : LOW);
  digitalWrite(Config::PIN_LED_GREEN, channelOutput(green_) ? HIGH : LOW);
  digitalWrite(Config::PIN_LED_RED, channelOutput(red_) ? HIGH : LOW);
}

void LedService::setGreenConnecting()
{
  setChannel(green_, Mode::Blink, true);
}

void LedService::setGreenConnected()
{
  setChannel(green_, Mode::Solid, true);
}

void LedService::setGreenFatal()
{
  setChannel(green_, Mode::Off, false);
}

void LedService::setRedDefault()
{
  setChannel(red_, Mode::Off, false);
}

void LedService::setRedMiniError()
{
  setChannel(red_, Mode::Blink, true);
}

void LedService::setRedMajorError()
{
  setChannel(red_, Mode::Solid, true);
}

void LedService::startBlockingProcess()
{
  setChannel(onboard_, Mode::Blink, true);
}

void LedService::stopBlockingProcess()
{
  setChannel(onboard_, Mode::Off, false);
}

void LedService::setFatalError()
{
  setGreenFatal();
  setRedMajorError();
  stopBlockingProcess();
}

void LedService::setChannel(Channel &channel, Mode mode, bool output)
{
  channel.mode = mode;
  channel.output = output;
  channel.blinkPhase = output;
  channel.lastToggleMs = millis();
}

void LedService::updateChannel(Channel &channel, unsigned long nowMs)
{
  if (channel.mode != Mode::Blink)
  {
    channel.output = (channel.mode == Mode::Solid);
    return;
  }

  if (nowMs - channel.lastToggleMs >= Config::LED_TOGGLE_INTERVAL_MS)
  {
    channel.blinkPhase = !channel.blinkPhase;
    channel.lastToggleMs = nowMs;
  }

  channel.output = channel.blinkPhase;
}

bool LedService::channelOutput(const Channel &channel) const
{
  return channel.output;
}
