#include <blink.h>

Blink::Blink(unsigned long intervalMs)
    : intervalMs_(intervalMs), lastToggleTime_(0), ledState_(false)
{
}

void Blink::update(unsigned long currentTime)
{
    if (currentTime - lastToggleTime_ >= intervalMs_)
    {
        ledState_ = !ledState_;
        lastToggleTime_ = currentTime;
    }
}

bool Blink::isOn() const
{
    return ledState_;
}