#pragma once

class Blink
{
public:
    explicit Blink(unsigned long intervalMs);

    void update(unsigned long currentTime);

    bool isOn() const;

private:
    unsigned long intervalMs_;
    unsigned long lastToggleTime_;
    bool ledState_;
};