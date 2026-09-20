#include <unity.h>
#include "test_suites.h"

int main()
{
    UNITY_BEGIN();

    // Call the suites located in separate files
    run_blink_tests();
    run_morse_tests();

    return UNITY_END();
}