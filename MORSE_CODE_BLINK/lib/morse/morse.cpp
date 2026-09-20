#include <morse.h>
#include <morseOutput.h>
#include <morseCode.h>

Morse::Morse(MorseOutput &output,
             int dotDuration,
             int dashDuration,
             int symbolGap,
             int letterGap,
             int wordGap)
    : output(output),
      dotDuration(dotDuration),
      dashDuration(dashDuration),
      symbolGap(symbolGap),
      letterGap(letterGap),
      wordGap(wordGap)
{
}

void Morse::dot()
{
    output.set(true);
    output.wait(dotDuration);
    output.set(false);
}

void Morse::dash()
{
    output.set(true);
    output.wait(dashDuration);
    output.set(false);
}

void Morse::send(const char *message)
{
    bool hasOutput = false;
    bool wordGapSent = false;

    for (int i = 0; message[i] != '\0'; i++)
    {
        if (message[i] == ' ')
        {
            if (hasOutput && !wordGapSent)
            {
                bool hasNextCharacter = false;

                for (int j = i + 1; message[j] != '\0'; j++)
                {
                    if (message[j] != ' ')
                    {
                        hasNextCharacter = true;
                        break;
                    }
                }

                if (hasNextCharacter)
                {
                    output.wait(wordGap);
                    wordGapSent = true;
                }
            }

            continue;
        }

        if (sendCharacter(message[i]))
        {
            hasOutput = true;
            wordGapSent = false;

            if (message[i + 1] != '\0' &&
                message[i + 1] != ' ')
            {
                output.wait(letterGap);
            }
        }
    }
}

bool Morse::sendCharacter(char character)
{
    if (character >= 'a' && character <= 'z')
    {
        character = character - 'a' + 'A';
    }

    const char *code = MorseCode::get(character);

    if (code == nullptr)
    {
        return false;
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
            output.wait(symbolGap);
        }
    }

    return true;
}