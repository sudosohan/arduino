#pragma once

class MorseOutput
{
public:
    virtual ~MorseOutput() = default;

    virtual void set(bool state) = 0;
    virtual void wait(unsigned long milliseconds) = 0;
};