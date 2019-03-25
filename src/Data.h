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

inline float pos_int_to_float(int val)
{
    // as the area 0-1023 is mapped to [-1,+1]:
    const float scalar = (1.f / 512);

    return val * scalar - 1.f;
}

class Data {
    constexpr static const float voltage_scale = 0.012713472f; /* Vmax = 13V -> 1023 */
    constexpr static const float current_scale = 0.003225806f; /* Imax = 3A3 -> 1023 */

    unsigned long _updated_at_millis;

public:
    Data(){};
    Data(uint16_t rpos, uint16_t rcur, uint16_t rvel, uint16_t rvolt, uint16_t rtemp)
        : raw_pos(rpos)
        , raw_current(rcur)
        , raw_velocity(rvel)
        , raw_voltage(rvolt)
        , raw_temp(rtemp){};
    ~Data(){};

    // raw position of the board, 0-65535 -> -1.0 – +1.0
    uint16_t raw_pos;

    // raw current registered by the board, 0-1023 -> 0A-3A3
    uint16_t raw_current;

    // raw velocity
    uint16_t raw_velocity;

    // raw voltage registered by the board, 0-1023 -> 0V-13V
    uint16_t raw_voltage;

    // raw temperature seen by the board, will be 0xFFFF if no thermistor is connected
    uint16_t raw_temp;

    float get_position()
    {
        return uint16_to_sc(raw_pos); // * direction + offset
    };

    float get_current()
    {
        return raw_current * current_scale;
    };

    float get_velocity()
    {
        return int16_to_sc(raw_velocity); // * direction
    }

    float get_voltage()
    {
        return raw_voltage * voltage_scale;
    };

    float get_temp()
    {
        if (raw_temp < 0) {
            return .0f;
        }

        return ((55 * raw_temp) - 25450) * 0.01;
    };
};

#endif /* __DATA_H */