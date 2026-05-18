#include "Logger.h"

namespace
{
  bool loggerReady = false;

  void printLine(const char *level, const String &message)
  {
    if (!loggerReady)
    {
      Serial.begin(115200);
      loggerReady = true;
    }

    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);
  }
} // namespace

namespace Logger
{
  void begin()
  {
    if (!loggerReady)
    {
      Serial.begin(115200);
      loggerReady = true;
    }
  }

  void warn(const String &message)
  {
    printLine("WARN", message);
  }

  void error(const String &message)
  {
    printLine("ERROR", message);
  }
} // namespace Logger
