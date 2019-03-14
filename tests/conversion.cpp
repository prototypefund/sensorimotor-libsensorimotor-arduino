#include "../src/utils.h"
#include <iostream>

int test_float_to_uint8()
{
    float input[] = { 0.f, 1.f, 0.5f };
    uint8_t expected[] = { 0, 255, 127 };

    int len = (sizeof(input) / sizeof(*input));
    uint8_t output = 0;

    for (uint8_t i = 0; i < len; i++) {
        output = float_to_uint8(input[i]);

        if (output != expected[i]) {
            std::cout << "Error in test case " << i + 1 << std::endl;
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
            std::cout << "Error in test case " << i + 1 << std::endl;
            printf("Value for %f was %d (expected %d)\n", input[i], output, expected[i]);
            return i + 1;
        }
    }

    return 0;
}

int main(void)
{
    if (test_float_to_uint8() != 0)
        return 1;

    if (test_float_to_int16() != 0)
        return 2;

    std::cout
        << "Tests successful! \n";
    return 0;
}