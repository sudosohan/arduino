#include <Arduino.h>
#include <morse.h>
#include <blink.h>
#include <arduinoMorseOutput.h>

#ifndef BLINK_INTERVAL_MS
#define BLINK_INTERVAL_MS 1000
#endif

Blink blink(BLINK_INTERVAL_MS);

constexpr int DOT_DURATION = 200;
constexpr int DASH_DURATION = 600;
constexpr int SYMBOL_GAP = 200;
constexpr int LETTER_GAP = 600;
constexpr int WORD_GAP = 1200;

ArduinoMorseOutput output(LED_BUILTIN);

Morse morse(
    output,
    DOT_DURATION,
    DASH_DURATION,
    SYMBOL_GAP,
    LETTER_GAP,
    WORD_GAP);

void setup()
{
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  // pinMode(LED_BUILTIN, OUTPUT);
  output.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // blink.update(millis());
  // digitalWrite(LED_BUILTIN, blink.isOn() ? HIGH : LOW);
  morse.send("SOS");
}
