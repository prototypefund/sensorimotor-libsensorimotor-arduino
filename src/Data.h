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
    ~Data(){};

    // raw position of the board, 0-1023
    int raw_pos;

    // raw current registered by the board, 0-1023 -> 0A-3A3
    int raw_current;

    // raw velocity
    int raw_velocity;

    // raw voltage registered by the board, 0-1023 -> 0V-13V
    int raw_voltage;

    // raw temperature seen by the board, will be 0xFFFF if no thermistor is connected
    int raw_temp;

    float get_position()
    {
        return pos_int_to_float(raw_pos);
    };

    float get_current()
    {
        return raw_current * current_scale;
    };

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