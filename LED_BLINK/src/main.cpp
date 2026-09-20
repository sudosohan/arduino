#include <Arduino.h>
#include "blink.h"

#ifndef BLINK_INTERVAL_MS
#define BLINK_INTERVAL_MS 1000
#endif

Blink blink(BLINK_INTERVAL_MS);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  blink.update(millis());
  digitalWrite(LED_BUILTIN, blink.isOn() ? HIGH : LOW);
}
