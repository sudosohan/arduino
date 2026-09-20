#include <unity.h>
#include <vector>
#include <morse.h>
#include <morseCode.h>
using namespace std;

class FakeMorseOutput : public MorseOutput
{
public:
    struct Event
    {
        enum Type
        {
            SET,
            WAIT
        };

        Type type;
        int value;
    };

    vector<Event> events;

    void set(bool state) override
    {
        events.push_back({Event::SET,
                          state ? 1 : 0});
    }

    void wait(unsigned long milliseconds) override
    {
        events.push_back({Event::WAIT,
                          static_cast<int>(milliseconds)});
    }
};

void test_s_is_three_dots()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("S");

    TEST_ASSERT_EQUAL(11, output.events.size());

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET,
                      output.events[0].type);
    TEST_ASSERT_EQUAL(1, output.events[0].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::WAIT,
                      output.events[1].type);
    TEST_ASSERT_EQUAL(200, output.events[1].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET,
                      output.events[2].type);
    TEST_ASSERT_EQUAL(0, output.events[2].value);
}

void test_o_is_three_dashes()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("O");

    // 3 events per dash + 2 symbol gaps
    TEST_ASSERT_EQUAL(11, output.events.size());

    // First dash
    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET, output.events[0].type);
    TEST_ASSERT_EQUAL(1, output.events[0].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::WAIT, output.events[1].type);
    TEST_ASSERT_EQUAL(600, output.events[1].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET, output.events[2].type);
    TEST_ASSERT_EQUAL(0, output.events[2].value);

    // Symbol gap
    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::WAIT, output.events[3].type);
    TEST_ASSERT_EQUAL(200, output.events[3].value);

    // Second dash
    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET, output.events[4].type);
    TEST_ASSERT_EQUAL(1, output.events[4].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::WAIT, output.events[5].type);
    TEST_ASSERT_EQUAL(600, output.events[5].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET, output.events[6].type);
    TEST_ASSERT_EQUAL(0, output.events[6].value);

    // Symbol gap
    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::WAIT, output.events[7].type);
    TEST_ASSERT_EQUAL(200, output.events[7].value);

    // Third dash
    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET, output.events[8].type);
    TEST_ASSERT_EQUAL(1, output.events[8].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::WAIT, output.events[9].type);
    TEST_ASSERT_EQUAL(600, output.events[9].value);

    TEST_ASSERT_EQUAL(FakeMorseOutput::Event::SET, output.events[10].type);
    TEST_ASSERT_EQUAL(0, output.events[10].value);
}

void test_sos_has_correct_letter_gaps()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("SOS");

    TEST_ASSERT_GREATER_THAN(0, output.events.size());

    int letterGapCount = 0;

    for (size_t i = 0; i + 1 < output.events.size(); i++)
    {
        const auto &current = output.events[i];
        const auto &next = output.events[i + 1];

        // A letter gap occurs after the LED has been turned OFF
        // and before the next symbol starts.
        if (current.type == FakeMorseOutput::Event::SET &&
            current.value == 0 &&
            next.type == FakeMorseOutput::Event::WAIT &&
            next.value == 600)
        {
            letterGapCount++;
        }
    }

    TEST_ASSERT_EQUAL(2, letterGapCount);
}

void test_words_have_correct_gap()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("SOS SOS");

    int wordGapCount = 0;

    for (size_t i = 0; i + 1 < output.events.size(); i++)
    {
        const auto &current = output.events[i];
        const auto &next = output.events[i + 1];

        if (current.type == FakeMorseOutput::Event::SET &&
            current.value == 0 &&
            next.type == FakeMorseOutput::Event::WAIT &&
            next.value == 1200)
        {
            wordGapCount++;
        }
    }

    TEST_ASSERT_EQUAL(1, wordGapCount);
}

void test_lowercase_sos()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("sos");

    TEST_ASSERT_EQUAL(35, output.events.size());
}

void test_unsupported_character_is_ignored()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("S$OS");

    TEST_ASSERT_EQUAL(35, output.events.size());
}

void test_multiple_spaces_create_one_word_gap()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("SOS  SOS");

    int wordGapCount = 0;

    for (const auto &event : output.events)
    {
        if (event.type == FakeMorseOutput::Event::WAIT && event.value == 1200)
        {
            wordGapCount++;
        }
    }

    TEST_ASSERT_EQUAL(1, wordGapCount);
}

void test_leading_spaces_are_ignored()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("  SOS");

    TEST_ASSERT_EQUAL(35, output.events.size());
}

void test_trailing_spaces_are_ignored()
{
    FakeMorseOutput output;

    Morse morse(
        output,
        200,
        600,
        200,
        600,
        1200);

    morse.send("SOS  ");

    TEST_ASSERT_EQUAL(35, output.events.size());
}

void test_morse_code_lookup()
{
    TEST_ASSERT_EQUAL_STRING("...", MorseCode::get('S'));
    TEST_ASSERT_EQUAL_STRING("---", MorseCode::get('O'));
}

void test_unsupported_morse_code_returns_null()
{
    TEST_ASSERT_NULL(MorseCode::get('$'));
}

void test_full_morse_alphabet()
{
    const char *expected[] = {
        ".-",   // A
        "-...", // B
        "-.-.", // C
        "-..",  // D
        ".",    // E
        "..-.", // F
        "--.",  // G
        "....", // H
        "..",   // I
        ".---", // J
        "-.-",  // K
        ".-..", // L
        "--",   // M
        "-.",   // N
        "---",  // O
        ".--.", // P
        "--.-", // Q
        ".-.",  // R
        "...",  // S
        "-",    // T
        "..-",  // U
        "...-", // V
        ".--",  // W
        "-..-", // X
        "-.--", // Y
        "--.."  // Z
    };

    for (char character = 'A'; character <= 'Z'; character++)
    {
        const char *actual = MorseCode::get(character);

        TEST_ASSERT_NOT_NULL(actual);
        TEST_ASSERT_EQUAL_STRING(
            expected[character - 'A'],
            actual);
    }
}

void run_morse_tests()
{
    RUN_TEST(test_s_is_three_dots);
    RUN_TEST(test_o_is_three_dashes);
    RUN_TEST(test_sos_has_correct_letter_gaps);
    RUN_TEST(test_words_have_correct_gap);
    RUN_TEST(test_lowercase_sos);
    RUN_TEST(test_unsupported_character_is_ignored);
    RUN_TEST(test_multiple_spaces_create_one_word_gap);
    RUN_TEST(test_leading_spaces_are_ignored);
    RUN_TEST(test_trailing_spaces_are_ignored);

    RUN_TEST(test_morse_code_lookup);
    RUN_TEST(test_unsupported_morse_code_returns_null);

    RUN_TEST(test_full_morse_alphabet);
}