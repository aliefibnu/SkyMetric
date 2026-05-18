#ifndef SKYMETRIC_LOGGER_H
#define SKYMETRIC_LOGGER_H

#include <Arduino.h>

namespace Logger
{
  void begin();
  void warn(const String &message);
  void error(const String &message);
} // namespace Logger

#endif
