#include <unity.h>

#include "blink.h"

void test_led_is_off_intially()
{
    Blink blink(1000);
    TEST_ASSERT_FALSE(blink.isOn());
}

void test_led_turn_on_after_interval()
{
    Blink blink(1000);
    blink.update(1000);
    TEST_ASSERT_TRUE(blink.isOn());
}

void test_led_turns_off_after_second_interval()
{
    Blink blink(1000);

    blink.update(1000);
    blink.update(2000);

    TEST_ASSERT_FALSE(blink.isOn());
}

void test_led_does_not_toggle_before_interval()
{
    Blink blink(1000);

    blink.update(999);

    TEST_ASSERT_FALSE(blink.isOn());
}

void test_led_toggles_every_interval()
{
    Blink blink(500);

    blink.update(500);
    TEST_ASSERT_TRUE(blink.isOn());

    blink.update(1000);
    TEST_ASSERT_FALSE(blink.isOn());

    blink.update(1500);
    TEST_ASSERT_TRUE(blink.isOn());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_led_is_off_intially);
    RUN_TEST(test_led_turn_on_after_interval);
    RUN_TEST(test_led_turns_off_after_second_interval);
    RUN_TEST(test_led_does_not_toggle_before_interval);
    RUN_TEST(test_led_toggles_every_interval);

    return UNITY_END();
}