#ifndef SKYMETRIC_LED_SERVICE_H
#define SKYMETRIC_LED_SERVICE_H

#include <Arduino.h>

class LedService
{
public:
  enum class Mode
  {
    Off,
    Solid,
    Blink
  };

  void begin();
  void update(unsigned long nowMs);
  String toStatusJson() const;

  void setGreenConnecting();
  void setGreenConnected();
  void setGreenFatal();

  void setRedDefault();
  void setRedMiniError();
  void setRedMajorError();

  void startBlockingProcess();
  void stopBlockingProcess();

  void setFatalError();

private:
  struct Channel
  {
    Mode mode = Mode::Off;
    bool output = false;
    bool blinkPhase = false;
    unsigned long lastToggleMs = 0;
  };

  Channel onboard_;
  Channel green_;
  Channel red_;

  void setChannel(Channel &channel, Mode mode, bool output);
  void updateChannel(Channel &channel, unsigned long nowMs);
  bool channelOutput(const Channel &channel) const;
  void applyState();
};

#endif
