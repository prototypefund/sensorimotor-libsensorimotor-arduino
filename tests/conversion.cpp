#include "../src/Data.h"
#include "../src/utils.h"
#include <cstdlib>
#include <stdio.h>

bool is_close_enough(float a, float b)
{
    a -= b;

    if (a > 0.005)
        return false;

    if (a < -0.005)
        return false;
    double x;

    return true;
}

int test_int16_to_sc()
{
    uint16_t input[] = { 0x0000, 0x8000, 0xFFFF };
    float expected[] = { 0.00f, -1.00f, 0.00f };

    int len = (sizeof(input) / sizeof(*input));
    float output = 0.0f;

    for (uint8_t i = 0; i < len; i++) {
        output = int16_to_sc(input[i]);

        if (!is_close_enough(output, expected[i])) {
            printf("Error in test case %d\n", i + 1);
            printf("Value for %04X was %.2f (expected %.2f)\n", input[i], output, expected[i]);
            return i + 1;
        }
    }

    return 0;
}

int test_uint16_to_sc()
{
    uint16_t input[] = { 0x0000, 0xFFFF, 0x8000 };
    float expected[] = { -1.00f, 1.0f, 0.00f };

    int len = (sizeof(input) / sizeof(*input));
    float output = 0.0f;

    for (uint8_t i = 0; i < len; i++) {
        output = uint16_to_sc(input[i]);

        if (!is_close_enough(output, expected[i])) {
            printf("Error in test case %d\n", i + 1);
            printf("Value for %04X was %.2f (expected %.2f)\n", input[i], output, expected[i]);
            return i + 1;
        }
    }

    return 0;
}

int test_float_to_uint8()
{
    float input[] = { 0.f, 1.f, 0.5f };
    uint8_t expected[] = { 0, 255, 127 };

    int len = (sizeof(input) / sizeof(*input));
    uint8_t output = 0;

    for (uint8_t i = 0; i < len; i++) {
        output = float_to_uint8(input[i]);

        if (output != expected[i]) {
            printf("Error in test case %d\n", i + 1);
            printf("Value for %f was %d (expected %d)\n", input[i], output, expected[i]);
            return i + 1;
        }
    }

    return 0;
}

int test_float_to_int16()
{
    float input[] = { 0.f, 1.f, -1.f, 0.5f, -0.5f };
    int16_t expected[] = { 0, 255, -255, 127, -127 };

    int len = (sizeof(input) / sizeof(*input));
    int16_t output = 0;

    for (uint8_t i = 0; i < len; i++) {
        output = float_to_int16(input[i]);

        if (output != expected[i]) {
            printf("Error in test case %d\n", i + 1);
            printf("Value for %f was %d (expected %d)\n", input[i], output, expected[i]);
            return i + 1;
        }
    }

    return 0;
}

int main(void)
{
    if (test_uint16_to_sc() != 0)
        return 1;

    if (test_int16_to_sc() != 0)
        return 2;

    if (test_float_to_uint8() != 0)
        return 3;

    if (test_float_to_int16() != 0)
        return 4;

    return 0;
}