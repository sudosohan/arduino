#include <Arduino.h>

#include "arduino_led_output.h"
#include "binary_display.h"

const uint8_t LED_PINS[] = {2, 3, 4, 5};

ArduinoLedOutput output(LED_PINS);

BinaryDisplay display(output);

void setup()
{
  display.begin();
}

void loop()
{
  display.run();
}