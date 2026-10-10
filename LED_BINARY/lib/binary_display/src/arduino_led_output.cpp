#include "arduino_led_output.h"

bool ArduinoLedOutput::begin()
{
    if (!valid_)
    {
        return false;
    }

    for (size_t i = 0; i < ledCount_; ++i)
    {
        pinMode(ledPins_[i], OUTPUT);
        digitalWrite(ledPins_[i], LOW);
    }

    return true;
}

void ArduinoLedOutput::setLed(size_t index, bool state)
{
    if (!valid_ || index >= ledCount_)
    {
        return;
    }

    digitalWrite(
        ledPins_[index],
        state ? HIGH : LOW);
}