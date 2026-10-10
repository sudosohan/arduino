#ifndef LED_OUTPUT_H
#define LED_OUTPUT_H

#include <stddef.h>
#include <stdint.h>

class LedOutput
{
public:
    virtual ~LedOutput() = default;

    virtual bool begin() = 0;

    virtual size_t ledCount() const = 0;

    virtual void setLed(size_t index, bool state) = 0;
};

#endif