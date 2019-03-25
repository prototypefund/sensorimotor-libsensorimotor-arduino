#include <cstdlib>
#include <stdint.h>
#include <stdio.h>

#include "../src/Data.h"

// message for testing
uint8_t msgbuf[] = {
    0xFF, // sync 1
    0xFF, // sync 2
    0x80, // command id 0x80 = data response
    0x03, // motor ID "3"
    0x99, // POS
    0x40, // POS 39232
    0x00, // CUR
    0x07, // CUR 7 * 0.003225806 = 22mA
    0x00, // VEL
    0x00, // VEL 0
    0x03, // Voltage
    0xAA, // Voltage 938 * 0.012713472 = 11.925V
    0xFF, // TEMP
    0xFF, // TEMP
    0xF4, // Checksum (valid)
};

int concat_word(int high_byte, int low_byte)
{
    return (((uint8_t)high_byte << 8) | ((uint8_t)low_byte & 0x00FF));
}

uint8_t checksum(uint8_t* buf, uint8_t len)
{
    uint8_t i = 0;
    uint8_t sum = 0x00;
    for (i = 0; i < len; i++) {
        sum += buf[i];
    }
    return (~sum + 1);
};

int test_checksum()
{

    uint8_t sum = checksum(msgbuf, 14);

    if (sum != msgbuf[14])
        return 2;

    return 0;
}

int test_concat()
{
    int hi = 0x11F0;
    int lo = 0x110D;

    if (concat_word(hi, lo) != 0xF00D)
        return 1;

    return 0;
}

int main()
{
    if (test_checksum() != 0)
        return 1;

    if (test_concat() != 0)
        return 2;

    return 0;
}