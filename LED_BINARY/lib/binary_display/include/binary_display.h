#ifndef BINARY_DISPLAY_H
#define BINARY_DISPLAY_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>
#include "led_output.h"

#ifndef BINARY_DISPLAY_INTERVAL
#define BINARY_DISPLAY_INTERVAL 500
#endif

class BinaryDisplay
{
public:
    explicit BinaryDisplay(
        LedOutput &output,
        unsigned long interval = BINARY_DISPLAY_INTERVAL,
        bool repeat = true);

    BinaryDisplay(
        LedOutput &output,
        uint32_t start,
        uint32_t end,
        unsigned long interval = BINARY_DISPLAY_INTERVAL,
        bool repeat = true);

    bool begin();

    bool isValid() const;

    bool show(uint32_t number);

    bool run();

private:
    LedOutput &output_;

    size_t ledCount_ = 0;

    uint32_t start_ = 0;
    uint32_t end_ = 0;

    unsigned long interval_ = BINARY_DISPLAY_INTERVAL;
    bool repeat_ = true;

    bool valid_ = false;
    bool begun_ = false;

    void initializeDefault();

    void initialize(
        uint32_t start,
        uint32_t end,
        unsigned long interval,
        bool repeat);

    uint32_t maxValue() const;
    static uint32_t maxValueFor(size_t ledCount);
};

#endif