#pragma once

#include <morseOutput.h>

class Morse
{
public:
    Morse(MorseOutput &output,
          int dotDuration,
          int dashDuration,
          int symbolGap,
          int letterGap,
          int wordGap);

    void send(const char *message);

private:
    void dot();
    void dash();
    bool sendCharacter(char character);

    MorseOutput &output;

    int dotDuration;
    int dashDuration;
    int symbolGap;
    int letterGap;
    int wordGap;
};