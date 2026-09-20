# Embedded Morse Code Blinker

A production-ready embedded Morse code blinker built with PlatformIO, following clean architecture principles and industry-level standards. The project supports Arduino Uno and ESP32 boards with a modular, testable design.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Architecture](#architecture)
- [Directory Structure](#directory-structure)
- [Component Design](#component-design)
- [Data Flow](#data-flow)
- [Morse Timing](#morse-timing)
- [Configuration](#configuration)
- [Build and Upload](#build-and-upload)
- [Testing](#testing)
- [Extending the Project](#extending-the-project)

---

## Project Overview

This project blinks an LED to transmit Morse code messages. It started as a simple SOS blinker and has been refactored into a modular, testable system that separates concerns cleanly:

- **Message encoding** (text to Morse symbols)
- **Signal output** (LED control abstraction)
- **Timing management** (non-blocking blink support)

The design uses dependency injection and abstract interfaces, making it easy to swap output backends (LED, serial, buzzer) without changing core logic.

---

## Architecture

### High-Level Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         main.cpp                                │
│  ┌────────────┐  ┌──────────────────┐  ┌────────────────────┐  │
│  │   Blink    │  │      Morse       │  │ ArduinoMorseOutput │  │
│  │ (utility)  │  │  (core engine)   │  │  (LED backend)     │  │
│  └─────┬──────┘  └────────┬─────────┘  └──────────┬─────────┘  │
│        │                  │                       │             │
│        │                  │ depends on            │             │
│        │                  ▼                       │             │
│        │         ┌────────────────┐               │             │
│        │         │  MorseOutput   │◄──────────────┘             │
│        │         │  (interface)   │                             │
│        │         └────────┬───────┘                             │
│        │                  │                                     │
│        │                  │ uses                                │
│        │                  ▼                                     │
│        │         ┌────────────────┐                             │
│        │         │   MorseCode    │                             │
│        │         │ (lookup table) │                             │
│        │         └────────────────┘                             │
└─────────────────────────────────────────────────────────────────┘
```

### Class Relationship Diagram (UML)

```
┌──────────────────┐       ┌──────────────────┐
│      Blink       │       │    MorseCode      │
├──────────────────┤       ├──────────────────┤
│ - intervalMs_    │       │ + get(char)      │
│ - lastToggleTime_│       │   : const char*  │
│ - ledState_      │       └──────────────────┘
├──────────────────┤
│ + update(time)   │       ┌──────────────────┐
│ + isOn() : bool  │       │   MorseOutput    │
└──────────────────┘       │   (interface)    │
                           ├──────────────────┤
                           │ + set(bool)      │
                           │ + wait(ms)       │
                           └────────┬─────────┘
                                    △
                                    │ implements
                           ┌────────┴─────────┐
                           │ ArduinoMorseOutput│
                           ├──────────────────┤
                           │ - pin            │
                           ├──────────────────┤
                           │ + begin()        │
                           │ + set(bool)      │
                           │ + wait(ms)       │
                           └──────────────────┘

┌──────────────────┐
│      Morse       │
├──────────────────┤
│ - output         │◄──── MorseOutput&
│ - dotDuration    │
│ - dashDuration   │
│ - symbolGap      │
│ - letterGap      │
│ - wordGap        │
├──────────────────┤
│ + send(msg)      │
│ - dot()          │
│ - dash()         │
│ - sendChar(c)    │
└──────────────────┘
```

### Design Patterns Used

| Pattern | Where | Purpose |
|---------|-------|---------|
| **Strategy** | `MorseOutput` interface | Swap output backends without changing `Morse` logic |
| **Dependency Injection** | `Morse` receives `MorseOutput&` | Decouples core logic from hardware |
| **Single Responsibility** | Each class has one job | `MorseCode` = lookup, `Morse` = encode, `ArduinoMorseOutput` = drive LED |
| **Template Method** | `ArduinoMorseOutput::set/wait` | Platform-specific implementation behind abstract interface |

---

## Directory Structure

```
MORSE_CODE_BLINK/
├── platformio.ini              # Project configuration (environments, flags)
├── src/
│   └── main.cpp                # Application entry point
├── include/                    # Project headers (unused, PlatformIO convention)
├── lib/
│   ├── blink/                  # Non-blocking blink utility
│   │   ├── blink.h
│   │   └── blink.cpp
│   └── morse/                  # Morse code engine
│       ├── morseOutput.h       # Abstract output interface
│       ├── arduinoMorseOutput.h # Arduino LED implementation
│       ├── morseCode.h         # Character → Morse lookup
│       ├── morseCode.cpp
│       ├── morse.h             # Core Morse encoder
│       └── morse.cpp
└── test/
    ├── test_native/            # PC-hosted unit tests (Unity)
    │   ├── main.cpp
    │   ├── test_suites.h
    │   ├── test_blink/
    │   │   └── test_main.cpp
    │   └── test_morse/
    │       └── test_main.cpp
    └── test_embedded/          # Hardware-on-target tests
        └── test_blink/
            └── test_main.cpp
```

---

## Component Design

### `MorseOutput` — Abstract Output Interface

```
┌─────────────────────────────────┐
│         MorseOutput             │
│─────────────────────────────────│
│  + set(bool state) = 0         │  ← Turn LED on/off
│  + wait(unsigned long ms) = 0  │  ← Block for duration
│  + ~MorseOutput() = default    │
└─────────────────────────────────┘
```

This abstract class defines the contract for any output backend. The `Morse` engine only interacts with this interface, making it hardware-agnostic.

### `ArduinoMorseOutput` — Arduino LED Implementation

```
┌─────────────────────────────────┐
│      ArduinoMorseOutput         │
│         : MorseOutput           │
│─────────────────────────────────│
│  - pin : int                    │
│─────────────────────────────────│
│  + begin()                      │  ← pinMode(pin, OUTPUT)
│  + set(bool) override           │  ← digitalWrite(pin, HIGH/LOW)
│  + wait(ms) override            │  ← delay(ms)
└─────────────────────────────────┘
```

### `MorseCode` — Character Lookup Table

```
┌─────────────────────────────────┐
│          MorseCode              │
│─────────────────────────────────│
│  + get(char) : const char*     │
│      Returns e.g. ".-" for 'A' │
│      Returns nullptr for '$'   │
└─────────────────────────────────┘
```

Maps A–Z to standard International Morse Code patterns. Returns `nullptr` for unsupported characters, which the `Morse` engine silently skips.

### `Morse` — Core Encoder

```
┌─────────────────────────────────┐
│            Morse                │
│─────────────────────────────────│
│  - output : MorseOutput&       │
│  - dotDuration, dashDuration   │
│  - symbolGap, letterGap, wordGap│
│─────────────────────────────────│
│  + send(const char* msg)       │
│  - dot()                        │
│  - dash()                       │
│  - sendCharacter(char)          │
└─────────────────────────────────┘
```

Iterates through each character of the message, looks up its Morse pattern, and drives the output. Handles:
- Case normalization (lowercase → uppercase)
- Symbol gaps between dots/dashes
- Letter gaps between characters
- Word gaps between spaces
- Multiple/leading/trailing spaces
- Unsupported characters (skipped)

### `Blink` — Non-Blocking Blink Utility

```
┌─────────────────────────────────┐
│            Blink                │
│─────────────────────────────────│
│  - intervalMs_ : unsigned long  │
│  - lastToggleTime_             │
│  - ledState_ : bool             │
│─────────────────────────────────│
│  + update(currentTime)          │  ← Call with millis()
│  + isOn() : bool                │
└─────────────────────────────────┘
```

Uses the `millis()` pattern for non-blocking timing. Useful when you need to blink without blocking the main loop (e.g., alongside other tasks).

---

## Data Flow

### Message Transmission Flow

```
     "SOS"
       │
       ▼
┌──────────────┐
│  Morse::send │
│   ("SOS")    │
└──────┬───────┘
       │
       │  for each character
       ▼
┌──────────────────┐
│ Morse::sendChar  │
│  (char 'S')      │
└──────┬───────────┘
       │
       │  lookup Morse pattern
       ▼
┌──────────────────┐
│ MorseCode::get   │
│  ('S') → "..."   │
└──────┬───────────┘
       │
       │  for each symbol in "..."
       ▼
┌──────────────────┐
│  Morse::dot()    │
│  or Morse::dash()│
└──────┬───────────┘
       │
       │  drive output
       ▼
┌──────────────────┐
│  MorseOutput     │
│  .set(true)      │  → LED ON
│  .wait(200ms)    │  → delay
│  .set(false)     │  → LED OFF
└──────────────────┘
```

### SOS Transmission Timeline

```
Time ──────────────────────────────────────────────────────────────►

Letter 'S'           Letter 'O'              Letter 'S'
┌───┐ ┌───┐ ┌───┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌───┐ ┌───┐ ┌───┐
│ ● │ │ ● │ │ ● │ │───● │ │───● │ │───● │ │ ● │ │ ● │ │ ● │
└───┘ └───┘ └───┘ └─────┘ └─────┘ └─────┘ └───┘ └───┘ └───┘
 200   200   200   600     600     600     200   200   200   1200
  ms    ms    ms    ms      ms      ms      ms    ms    ms    ms

 ◄── letterGap ──► ◄── letterGap ──► ◄── wordGap (repeat) ──►
    (600ms)           (600ms)            (1200ms)
```

### Event Sequence for "SOS"

```
┌─────┬──────┬───────┬──────────────────────────────────────────┐
│ #   │ Type │ Value │ Description                              │
├─────┼──────┼───────┼──────────────────────────────────────────┤
│  1  │ SET  │   1   │ LED ON (dot 1 of S)                     │
│  2  │ WAIT │ 200   │ dot duration                             │
│  3  │ SET  │   0   │ LED OFF                                  │
│  4  │ WAIT │ 200   │ symbol gap                               │
│  5  │ SET  │   1   │ LED ON (dot 2 of S)                     │
│  6  │ WAIT │ 200   │ dot duration                             │
│  7  │ SET  │   0   │ LED OFF                                  │
│  8  │ WAIT │ 200   │ symbol gap                               │
│  9  │ SET  │   1   │ LED ON (dot 3 of S)                     │
│ 10  │ WAIT │ 200   │ dot duration                             │
│ 11  │ SET  │   0   │ LED OFF                                  │
│ 12  │ WAIT │ 600   │ letter gap (S → O)                       │
│ 13  │ SET  │   1   │ LED ON (dash 1 of O)                    │
│ 14  │ WAIT │ 600   │ dash duration                            │
│ 15  │ SET  │   0   │ LED OFF                                  │
│ 16  │ WAIT │ 200   │ symbol gap                               │
│ 17  │ SET  │   1   │ LED ON (dash 2 of O)                    │
│ 18  │ WAIT │ 600   │ dash duration                            │
│ 19  │ SET  │   0   │ LED OFF                                  │
│ 20  │ WAIT │ 200   │ symbol gap                               │
│ 21  │ SET  │   1   │ LED ON (dash 3 of O)                    │
│ 22  │ WAIT │ 600   │ dash duration                            │
│ 23  │ SET  │   0   │ LED OFF                                  │
│ 24  │ WAIT │ 600   │ letter gap (O → S)                       │
│ 25  │ SET  │   1   │ LED ON (dot 1 of S)                     │
│ 26  │ WAIT │ 200   │ dot duration                             │
│ 27  │ SET  │   0   │ LED OFF                                  │
│ 28  │ WAIT │ 200   │ symbol gap                               │
│ 29  │ SET  │   1   │ LED ON (dot 2 of S)                     │
│ 30  │ WAIT │ 200   │ dot duration                             │
│ 31  │ SET  │   0   │ LED OFF                                  │
│ 32  │ WAIT │ 200   │ symbol gap                               │
│ 33  │ SET  │   1   │ LED ON (dot 3 of S)                     │
│ 34  │ WAIT │ 200   │ dot duration                             │
│ 35  │ SET  │   0   │ LED OFF                                  │
└─────┴──────┴───────┴──────────────────────────────────────────┘
```

### Blink (Non-Blocking) State Machine

```
                    ┌──────────────────┐
                    │   LED OFF        │
                    │  (initial state) │
                    └────────┬─────────┘
                             │
                             │ update() when
                             │ elapsed >= interval
                             ▼
                    ┌──────────────────┐
                    │   LED ON         │
                    └────────┬─────────┘
                             │
                             │ update() when
                             │ elapsed >= interval
                             ▼
                    ┌──────────────────┐
                    │   LED OFF        │
                    └────────┬─────────┘
                             │
                             │ ... continues toggling
                             ▼
```

---

## Morse Timing

### Standard Timing Rules

| Element | Duration | Visual |
|---------|----------|--------|
| Dot (dit) | 200 ms | `●` |
| Dash (dah) | 600 ms | `───●` |
| Intra-character gap | 200 ms | space between dots/dashes |
| Inter-character gap | 600 ms | space between letters |
| Word gap | 1200 ms | space between words |

### Timing Relationships

```
1 dot = 1 unit = 200ms
1 dash = 3 units = 600ms
symbol gap = 1 unit = 200ms
letter gap = 3 units = 600ms
word gap = 7 units = 1200ms
```

### Morse Alphabet Reference

```
A  .-        N  -.        0  -----
B  -...      O  ---       1  .----
C  -.-.      P  .--.      2  ..---
D  -..       Q  --.-      3  ...--
E  .         R  .-.       4  ....-
F  ..-.      S  ...       5  .....
G  --.       T  -         6  -....
H  ....      U  ..-       7  --...
I  ..        V  ...-      8  ---..
J  .---      W  .--       9  ----.
K  -.-       X  -..-
L  .-..      Y  -.--
M  --        Z  --..
```

---

## Configuration

### PlatformIO Environments

The project defines three build environments in `platformio.ini`:

```
┌─────────────┬────────────────┬────────────┬──────────────┐
│ Environment │ Platform       │ Board      │ Framework    │
├─────────────┼────────────────┼────────────┼──────────────┤
│ uno         │ atmelavr       │ uno        │ arduino      │
│ esp32dev    │ espressif32    │ esp32dev   │ arduino      │
│ native      │ native         │ (host PC)  │ unity (test) │
└─────────────┴────────────────┴────────────┴──────────────┘
```

### Build Flags

| Flag | Value | Purpose |
|------|-------|---------|
| `-D LED_BUILTIN=13` | 13 (Uno), 2 (ESP32) | Override built-in LED pin |
| `-D BLINK_INTERVAL_MS=1000` | 1000 ms | Blink toggle interval |
| `-D UNIT_TEST` | (native env only) | Enable test-specific code |

### Upload & Monitor Configuration

| Setting | Uno | ESP32 |
|---------|-----|-------|
| Upload Port | `/dev/cu.usbmodem111101` | `/dev/cu.usbserial-0001` |
| Upload Protocol | default | esptool |
| Monitor Speed | 9600 baud | 115200 baud |

### Compile-Time Constants (main.cpp)

```cpp
constexpr int DOT_DURATION    = 200;   // ms
constexpr int DASH_DURATION   = 600;   // ms
constexpr int SYMBOL_GAP      = 200;   // ms
constexpr int LETTER_GAP      = 600;   // ms
constexpr int WORD_GAP        = 1200;  // ms
```

---

## Build and Upload

### Prerequisites

- [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation.html) or PlatformIO IDE extension for VS Code
- USB cable connected to the target board

### Commands

```bash
# Build for Arduino Uno
pio run -e uno

# Build for ESP32
pio run -e esp32dev

# Upload to Uno
pio run -e uno --target upload

# Upload to ESP32
pio run -e esp32dev --target upload

# Monitor serial output (Uno)
pio device monitor -e uno

# Monitor serial output (ESP32)
pio device monitor -e esp32dev

# Run native unit tests (on host PC)
pio test -e native
```

### Default Environments

Running `pio run` without `-e` builds both `uno` and `esp32dev` (configured as `default_envs` in `platformio.ini`).

---

## Testing

The project has two test suites:

### 1. Native Tests (Host PC)

Run on your development machine without hardware. Uses the Unity test framework.

```
┌─────────────────────────────────────────────────────┐
│                  test_native/                        │
│                                                     │
│  main.cpp ──► test_suites.h                         │
│                  │                                   │
│                  ├──► run_blink_tests()              │
│                  │      test_led_is_off_initially    │
│                  │      test_led_turn_on_after_int   │
│                  │      test_led_turns_off_after_2   │
│                  │      test_led_does_not_toggle     │
│                  │      test_led_toggles_every_int   │
│                  │                                   │
│                  └──► run_morse_tests()              │
│                         test_s_is_three_dots         │
│                         test_o_is_three_dashes       │
│                         test_sos_letter_gaps         │
│                         test_words_correct_gap       │
│                         test_lowercase_sos           │
│                         test_unsupported_char        │
│                         test_multiple_spaces         │
│                         test_leading_spaces          │
│                         test_trailing_spaces         │
│                         test_morse_code_lookup       │
│                         test_unsupported_code_null   │
│                         test_full_morse_alphabet     │
└─────────────────────────────────────────────────────┘
```

#### FakeMorseOutput (Test Double)

Native tests use a `FakeMorseOutput` class that records all `set()` and `wait()` calls as events, allowing tests to verify the exact sequence of LED operations without hardware:

```
┌──────────────────────────────────────┐
│         FakeMorseOutput              │
│         : MorseOutput                │
├──────────────────────────────────────┤
│  events : vector<Event>              │
│──────────────────────────────────────│
│  Event {                             │
│    type: SET | WAIT                  │
│    value: 0/1 (SET) or ms (WAIT)    │
│  }                                   │
│──────────────────────────────────────│
│  + set(bool)  → records SET event    │
│  + wait(ms)   → records WAIT event   │
└──────────────────────────────────────┘
```

### 2. Embedded Tests (On-Target)

Run on the actual Arduino/ESP32 hardware. Tests basic LED pin configuration and state.

| Test | What It Verifies |
|------|------------------|
| `test_led_pin_configured_as_output` | `pinMode()` succeeds |
| `test_led_state_high` | `digitalWrite(HIGH)` reads as HIGH |
| `test_led_state_low` | `digitalWrite(LOW)` reads as LOW |

### Running Tests

```bash
# Run native tests only
pio test -e native

# Run embedded tests on connected Uno
pio test -e uno

# Run embedded tests on connected ESP32
pio test -e esp32dev
```

---

## Extending the Project

### Adding a New Output Backend

Implement the `MorseOutput` interface:

```cpp
class BuzzerMorseOutput : public MorseOutput {
public:
    void set(bool state) override {
        // Drive a buzzer instead of LED
    }
    void wait(unsigned long ms) override {
        delay(ms);
    }
};
```

Then swap it in `main.cpp`:

```cpp
BuzzerMorseOutput output(PIN_BUZZER);
Morse morse(output, DOT_DURATION, DASH_DURATION, ...);
```

### Adding New Characters

Add cases to the switch statement in `lib/morse/morseCode.cpp`:

```cpp
case '0':
    return "-----";
case '1':
    return ".----";
```

### Using Non-Blocking Blink

Use the `Blink` class for concurrent tasks:

```cpp
void loop() {
    blink.update(millis());
    digitalWrite(LED_BUILTIN, blink.isOn() ? HIGH : LOW);
    // Other non-blocking tasks here...
}
```
