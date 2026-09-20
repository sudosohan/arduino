#pragma once

#include <Arduino.h>

class Morse
{
public:
    Morse(int dotDuration, int dashDuration, int symbolGap, int letterGap, int wordGap);
    void send(const char *code);

private:
    void dot();
    void dash();
    void sendCharacter(char character);

    int dotDuration;
    int dashDuration;
    int symbolGap;
    int letterGap;
    int wordGap;
};