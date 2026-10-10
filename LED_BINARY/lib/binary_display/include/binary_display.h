#ifndef BINARY_DISPLAY_H
#define BINARY_DISPLAY_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

#ifndef BINARY_DISPLAY_INTERVAL
#define BINARY_DISPLAY_INTERVAL 500
#endif

class BinaryDisplay
{
public:
    template <size_t N>
    explicit BinaryDisplay(const uint8_t (&ledPins)[N])
    {
        initializeDefault(ledPins, N);
    }

    template <size_t N>
    BinaryDisplay(
        const uint8_t (&ledPins)[N],
        uint32_t start,
        uint32_t end,
        unsigned long interval = BINARY_DISPLAY_INTERVAL,
        bool repeat = true)
    {
        initialize(ledPins, N, start, end, interval, repeat);
    }

    bool begin();

    bool isValid() const;

    bool show(uint32_t number);

    bool run();

private:
    static constexpr size_t MAX_LEDS = 32;

    uint8_t ledPins_[MAX_LEDS] = {};
    size_t ledCount_ = 0;

    uint32_t start_ = 0;
    uint32_t end_ = 0;

    unsigned long interval_ = BINARY_DISPLAY_INTERVAL;
    bool repeat_ = true;

    bool valid_ = false;
    bool begun_ = false;

    void initializeDefault(
        const uint8_t *ledPins,
        size_t ledCount);

    void initialize(
        const uint8_t *ledPins,
        size_t ledCount,
        uint32_t start,
        uint32_t end,
        unsigned long interval,
        bool repeat);

    static uint32_t maxValueFor(size_t ledCount);
    uint32_t maxValue() const;
};

#endif