#include <stdint.h>

#ifndef __UTILS_H
#define __UTILS_H

/*
 * `float_to_int16` maps the range [-1,+1] to the integer range
 * [-255,255]
 */
inline int16_t float_to_int16(float val)
{
    int16_t pwm = val * 255;
    if (pwm > 255)
        pwm = 255;
    else if (pwm < -255)
        pwm = -255;

    return pwm;
};

/*
 * `float_to_uint8` maps the range [0,+1] to the integer range
 * [0,255]
 */
inline uint8_t float_to_uint8(float val)
{
    uint16_t pwm = val * 255;
    if (pwm > 255)
        pwm = 255;
    else if (pwm < 0)
        pwm = 0;

    return pwm;
};

#endif /* __UTILS_H */