#ifndef ARDUINO_LED_OUTPUT_H
#define ARDUINO_LED_OUTPUT_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

#include "led_output.h"

class ArduinoLedOutput : public LedOutput
{
public:
    template <size_t N>
    ArduinoLedOutput(const uint8_t (&ledPins)[N]) : ledCount_(N)
    {
        static_assert(N > 0, "At least one LED is required");
        static_assert(N <= MAX_LEDS, "Too many LEDs");

        for (size_t i = 0; i < N; ++i)
        {
            ledPins_[i] = ledPins[i];
        }

        valid_ = true;
    };

    bool begin() override;

    size_t ledCount() const override
    {
        return ledCount_;
    }

    void setLed(size_t index, bool state) override;

private:
    static constexpr size_t MAX_LEDS = 32;

    uint8_t ledPins_[MAX_LEDS] = {};
    size_t ledCount_ = 0;
    bool valid_ = false;
};

#endif