#include <stdint.h>

#ifndef __BOARD_H
#define __BOARD_H

inline float uint16_to_sc(uint16_t val)
{
    return (val - 32768) / 32768.f;
}
inline float int16_to_sc(uint16_t val)
{
    return (int16_t)val / 32768.f;
}

class Board {

    constexpr static const float voltage_scale = 0.012713472f; /* Vmax = 13V -> 1023 */
    constexpr static const float current_scale = 0.003225806f; /* Imax = 3A3 -> 1023 */

    // id of the board, 0-127
    uint8_t _id;
    // raw position of the board, 0-1023
    uint16_t _raw_pos;
    // raw current registered by the board, 0-1023 -> 0A-3A3
    uint16_t _raw_current;
    // raw velocity
    uint16_t _raw_velocity;
    // raw voltage registered by the board, 0-1023 -> 0V-13V
    uint16_t _raw_voltage;
    // raw temperature seen by the board, will be 0xFFFF if no thermistor is connected
    uint16_t _raw_temp;
    unsigned long _last_milis;

    // PID specific
    bool use_pid;
    float last_error;

public:
    Board(uint8_t id)
        : _id(id){};
    Board()
        : _id(0xFF){}; // 0xFF = empty id
    ~Board(){};

    uint8_t ID()
    {
        return _id;
    }

    bool is_valid()
    {
        return (_id <= 127);
    }
};

#endif /* __BOARD_H */
