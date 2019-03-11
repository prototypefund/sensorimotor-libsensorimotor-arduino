#include <stdint.h>

#ifndef __DATA_H
#define __DATA_H

inline float uint16_to_sc(uint16_t val)
{
    return (val - 32768) / 32768.f;
}
inline float int16_to_sc(uint16_t val)
{
    return (int16_t)val / 32768.f;
}

class Data {
    constexpr static const float voltage_scale = 0.012713472f; /* Vmax = 13V -> 1023 */
    constexpr static const float current_scale = 0.003225806f; /* Imax = 3A3 -> 1023 */

public:
    Data(){};
    ~Data(){};

    // raw position of the board, 0-1023
    uint16_t raw_pos;

    // raw current registered by the board, 0-1023 -> 0A-3A3
    uint16_t raw_current;

    // raw velocity
    uint16_t raw_velocity;

    // raw voltage registered by the board, 0-1023 -> 0V-13V
    uint16_t raw_voltage;

    // raw temperature seen by the board, will be 0xFFFF if no thermistor is connected
    uint16_t raw_temp;

    unsigned long _updated_at_millis;
};

#endif /* __DATA_H */