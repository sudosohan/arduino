#include "binary_display.h"

#include <limits.h>

// Construct a display with the default range.
BinaryDisplay::BinaryDisplay(
    LedOutput &output,
    unsigned long interval,
    bool repeat)
    : output_(output)
{
    initializeDefault();

    interval_ = interval;
    repeat_ = repeat;

    if (interval_ == 0)
    {
        valid_ = false;
    }
}

// Construct a display with a custom range.
BinaryDisplay::BinaryDisplay(
    LedOutput &output,
    uint32_t start,
    uint32_t end,
    unsigned long interval,
    bool repeat)
    : output_(output)
{
    initialize(start, end, interval, repeat);
}

// Initialize the default range: 0 to maximum representable value.
void BinaryDisplay::initializeDefault()
{
    ledCount_ = output_.ledCount();

    if (ledCount_ == 0 || ledCount_ > 32)
    {
        valid_ = false;
        return;
    }

    start_ = 0;
    end_ = maxValue();

    valid_ = true;
}

// Initialize a custom range and timing configuration.
void BinaryDisplay::initialize(
    uint32_t start,
    uint32_t end,
    unsigned long interval,
    bool repeat)
{
    ledCount_ = output_.ledCount();

    if (ledCount_ == 0 || ledCount_ > 32)
    {
        valid_ = false;
        return;
    }

    start_ = start;
    end_ = end;
    interval_ = interval;
    repeat_ = repeat;

    if (interval_ == 0 || start_ > end_ || end_ > maxValue())
    {
        valid_ = false;
        return;
    }

    valid_ = true;
}

// Calculate the maximum representable value.
uint32_t BinaryDisplay::maxValue() const
{
    return maxValueFor(ledCount_);
}

// Calculate the maximum for a given number of LEDs.
uint32_t BinaryDisplay::maxValueFor(size_t ledCount)
{
    if (ledCount == 32)
    {
        return UINT32_MAX;
    }

    return (static_cast<uint32_t>(1) << ledCount) - 1;
}

// Initialize the underlying output.
bool BinaryDisplay::begin()
{
    if (!valid_)
    {
        return false;
    }

    begun_ = output_.begin();

    return begun_;
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
        const bool bitIsSet =
            (number & (static_cast<uint32_t>(1) << i)) != 0;

        output_.setLed(i, bitIsSet);
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