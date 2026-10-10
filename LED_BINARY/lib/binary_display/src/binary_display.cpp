#include "binary_display.h"

#include <limits.h>

// Initialize the display with its default range: 0 to maximum value.
void BinaryDisplay::initializeDefault(
    const uint8_t *ledPins,
    size_t ledCount)
{
    if (ledCount == 0 || ledCount > MAX_LEDS || ledPins == nullptr)
    {
        valid_ = false;
        return;
    }

    initialize(
        ledPins,
        ledCount,
        0,
        maxValueFor(ledCount),
        BINARY_DISPLAY_INTERVAL,
        true);
}

// Initialize the display with the supplied configuration.
void BinaryDisplay::initialize(
    const uint8_t *ledPins,
    size_t ledCount,
    uint32_t start,
    uint32_t end,
    unsigned long interval,
    bool repeat)
{
    valid_ = false;
    begun_ = false;

    if (ledPins == nullptr || ledCount == 0 || ledCount > MAX_LEDS)
    {
        return;
    }

    ledCount_ = ledCount;

    for (size_t i = 0; i < ledCount_; ++i)
    {
        ledPins_[i] = ledPins[i];
    }

    start_ = start;
    end_ = end;
    interval_ = interval;
    repeat_ = repeat;

    if (interval_ == 0 || start_ > end_ || end_ > maxValue())
    {
        return;
    }

    valid_ = true;
}

uint32_t BinaryDisplay::maxValueFor(size_t ledCount)
{
    if (ledCount == 32)
    {
        return UINT32_MAX;
    }

    return (static_cast<uint32_t>(1) << ledCount) - 1;
}

// Return the largest number that can be represented by the LEDs.
uint32_t BinaryDisplay::maxValue() const
{
    return maxValueFor(ledCount_);
}

// Configure the LED pins as outputs and turn all LEDs off.
bool BinaryDisplay::begin()
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

    begun_ = true;

    return true;
}

// Return whether the configuration is valid.
bool BinaryDisplay::isValid() const
{
    return valid_;
}

// Display one number in binary.
bool BinaryDisplay::show(uint32_t number)
{
    if (!valid_ || !begun_ || number > maxValue())
    {
        return false;
    }

    for (size_t i = 0; i < ledCount_; ++i)
    {
        const bool bitIsSet = (number & (static_cast<uint32_t>(1) << i)) != 0;

        digitalWrite(
            ledPins_[i],
            bitIsSet ? HIGH : LOW);
    }

    return true;
}

// Display the configured range, optionally repeating forever.
bool BinaryDisplay::run()
{
    if (!valid_ || !begun_)
    {
        return false;
    }

    do
    {
        for (uint32_t number = start_;; ++number)
        {
            if (!show(number))
            {
                return false;
            }

            delay(interval_);

            if (number == end_)
            {
                break;
            }
        }
    } while (repeat_);

    return true;
}