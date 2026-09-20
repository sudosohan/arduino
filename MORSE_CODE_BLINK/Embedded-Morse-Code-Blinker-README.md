# Embedded Morse Code Blinker

A simple embedded project using only an Arduino or ESP32 board and its built-in LED.

This project helps you practice digital output, timing, functions, loops, and basic embedded thinking without needing extra components.

## Project Goal

Make the built-in LED blink a message in Morse code.

Start with the message:

```text
SOS
```

In Morse code:

```text
S = dot dot dot
O = dash dash dash
S = dot dot dot
```

## Components Required

- Arduino board or ESP32 board
- USB cable
- Arduino IDE

No external LED, resistor, breadboard, or sensor is required.

## Concepts Practiced

- Digital output
- Built-in LED control
- Timing with `delay()`
- Functions
- Repeated patterns
- Basic embedded program structure
- Thinking in signals and states

## Built-In LED Pin

For many Arduino boards, the built-in LED is on:

```cpp
LED_BUILTIN
```

For some ESP32 boards, `LED_BUILTIN` may not work. Common built-in LED pins are:

```cpp
2
```

If your ESP32 LED does not blink, try changing:

```cpp
int ledPin = LED_BUILTIN;
```

to:

```cpp
int ledPin = 2;
```

## Morse Timing Rules

Use these timings:

```text
Dot blink       = 200 ms
Dash blink      = 600 ms
Gap inside letter = 200 ms
Gap between letters = 600 ms
Gap between words   = 1200 ms
```

## Basic Behavior

The LED should repeatedly blink:

```text
S O S
```

Pattern:

```text
S: short short short
O: long long long
S: short short short
```

Then pause and repeat.

## Suggested File Name

```text
MorseCodeBlinker.ino
```

## Starter Code

```cpp
int ledPin = LED_BUILTIN;

int dotDuration = 200;
int dashDuration = 600;
int symbolGap = 200;
int letterGap = 600;
int wordGap = 1200;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  blinkS();
  delay(letterGap);

  blinkO();
  delay(letterGap);

  blinkS();
  delay(wordGap);
}

void dot() {
  digitalWrite(ledPin, HIGH);
  delay(dotDuration);
  digitalWrite(ledPin, LOW);
  delay(symbolGap);
}

void dash() {
  digitalWrite(ledPin, HIGH);
  delay(dashDuration);
  digitalWrite(ledPin, LOW);
  delay(symbolGap);
}

void blinkS() {
  dot();
  dot();
  dot();
}

void blinkO() {
  dash();
  dash();
  dash();
}
```

## Step-by-Step Build Plan

### Step 1: Blink the LED normally

First upload a simple blink program to confirm your board and LED pin are working.

Expected behavior:

```text
LED turns ON
LED turns OFF
Repeats forever
```

### Step 2: Create dot and dash functions

Create two functions:

```cpp
void dot()
void dash()
```

The dot should be a short blink.

The dash should be a long blink.

### Step 3: Create letter functions

Create:

```cpp
void blinkS()
void blinkO()
```

`blinkS()` should call `dot()` three times.

`blinkO()` should call `dash()` three times.

### Step 4: Blink SOS

Inside `loop()`, call:

```cpp
blinkS();
blinkO();
blinkS();
```

Add delays between letters so the pattern is readable.

### Step 5: Repeat the message

After blinking `SOS`, add a longer delay before repeating.

## Expected Output

The LED should blink like this:

```text
short short short
long long long
short short short
pause
repeat
```

## Extra Challenges

Try these after the basic version works:

- Change the message from `SOS` to your initials
- Add more Morse letters, such as `A`, `B`, `C`, and `D`
- Create a function called `blinkMessage()`
- Make the LED blink faster or slower using timing variables
- Print the current letter to the Serial Monitor
- For ESP32, use PWM to make the LED fade for dots and dashes

## Optional Serial Monitor Improvement

You can print what the board is blinking:

```cpp
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}
```

Then inside `loop()`:

```cpp
Serial.println("Blinking S");
blinkS();

Serial.println("Blinking O");
blinkO();

Serial.println("Blinking S");
blinkS();
```

## Common Problems

### LED does not blink

Try changing the LED pin.

For Arduino:

```cpp
int ledPin = LED_BUILTIN;
```

For ESP32:

```cpp
int ledPin = 2;
```

### Upload fails

Check:

- Correct board is selected in Arduino IDE
- Correct port is selected
- USB cable supports data, not only charging
- ESP32 board package is installed if using ESP32

### Blink is too fast or too slow

Change these values:

```cpp
int dotDuration = 200;
int dashDuration = 600;
```

## Learning Reminder

This project looks simple, but it teaches an important embedded idea: software controls real-world signals over time.

Once you are comfortable with this, adding buttons, sensors, displays, and communication modules will feel much easier.
