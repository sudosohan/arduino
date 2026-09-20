#pragma once

#include <Arduino.h>
#include <morseOutput.h>

class ArduinoMorseOutput : public MorseOutput
{
public:
    explicit ArduinoMorseOutput(int pin)
        : pin(pin)
    {
    }

    void begin()
    {
        pinMode(pin, OUTPUT);
    }

    void set(bool state) override
    {
        digitalWrite(pin, state ? HIGH : LOW);
    }

    void wait(unsigned long milliseconds) override
    {
        delay(milliseconds);
    }

private:
    int pin;
};