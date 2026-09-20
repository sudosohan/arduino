#include "morse.h"
#include "Morse.h"

Morse::Morse(int dotDuration, int dashDuration, int symbolGap, int letterGap, int wordGap) : dotDuration(dotDuration), dashDuration(dashDuration), symbolGap(symbolGap), letterGap(letterGap), wordGap(wordGap)
{
}

void Morse::dot()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(dotDuration);

    digitalWrite(LED_BUILTIN, LOW);
}

void Morse::dash()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(dashDuration);

    digitalWrite(LED_BUILTIN, LOW);
}

void Morse::send(const char *message)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        if (message[i] == ' ')
        {
            delay(wordGap);
            continue;
        }

        sendCharacter(message[i]);

        if (message[i + 1] != '\0' &&
            message[i + 1] != ' ')
        {
            delay(letterGap);
        }
    }
}

void Morse::sendCharacter(char character)
{
    const char *code = nullptr;
    switch (character)
    {
    case 'S':
        code = "...";
        ;
        break;

    case 'O':
        code = "---";
        break;
    }

    if (code == nullptr)
    {
        return;
    }

    for (int i = 0; code[i] != '\0'; i++)
    {
        if (code[i] == '.')
        {
            dot();
        }
        else if (code[i] == '-')
        {
            dash();
        }

        // Gap between symbols
        if (code[i + 1] != '\0')
        {
            delay(symbolGap);
        }
    }
}