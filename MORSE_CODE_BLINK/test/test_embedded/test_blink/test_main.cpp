#include <Arduino.h>
#include <unity.h>

void test_led_pin_configured_as_output()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void test_led_state_high()
{
    digitalWrite(LED_BUILTIN, HIGH);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_BUILTIN));
}

void test_led_state_low()
{
    digitalWrite(LED_BUILTIN, LOW);

    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_BUILTIN));
}

void setup()
{
    delay(2000);

    pinMode(LED_BUILTIN, OUTPUT);

    UNITY_BEGIN();

    RUN_TEST(test_led_pin_configured_as_output);
    RUN_TEST(test_led_state_high);
    RUN_TEST(test_led_state_low);

    UNITY_END();
}

void loop()
{
}