#include <Arduino.h>
#include "binary_display.h"

const uint8_t LED_PINS[] = {2, 3, 4, 5};

BinaryDisplay display(LED_PINS);

void setup()
{
  display.begin();
}

void loop()
{
  display.run();
}
