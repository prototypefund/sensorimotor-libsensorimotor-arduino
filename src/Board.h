#include "Data.h"
#include <stdint.h>

#ifndef __BOARD_H
#define __BOARD_H

// TODO: where to put this?
const int motors_update_rate_hz = 100;

enum controller_t : uint8_t {
    CONTROLLER_DISABLED,
    CONTROLLER_PID,
    CONTROLLER_CONSTP,
    CONTROLLER_SPEED,
    CONTROLLER_IMPULSE
};

inline int8_t float_to_int8(float val)
{
    if (val > 1.f) {
        val = 1.f;
    } else if (val < -1.f) {
        val = -1.f;
    }

    return int8_t(val * 128 - 128);
};

class Board {

    // id of the board, 0-127
    uint8_t _id;

    // embedded data about position, voltage, current etc.
    Data data;

    controller_t controller = CONTROLLER_DISABLED;

    // PID specific
    PID pid_control;

    float last_error;

public:
    Board(uint8_t id)
        : _id(id)
        , pid_control(id, 1.f / motors_update_rate_hz){};
    Board(void)
        : _id(0xFF) // 0xFF = empty id
        , pid_control(0xFF, 10.0f){};
    ~Board(){};

    void set_data(Data d) { data = d; };

    Data get_data() { return data; };

    /* get_position is a shorthand for get_data().raw_pos */
    int get_position()
    {
        return data.raw_pos;
    };

    /* get_current is a shorthand for get_data().get_current() */
    int get_current() { return data.get_current(); };

    /* get_velocity is a shorthand for get_data().raw_velocity */
    int get_velocity() { return data.raw_velocity; };

    /* get_voltage is a shorthand for get_data().get_voltage */
    int get_voltage() { return data.get_voltage(); };

    /* get_temp is a shorthand for get_data().get_temp() */
    int get_temp() { return data.get_temp(); };

    uint8_t ID()
    {
        return _id;
    }

    /*
     * `calculate_voltage` returns the calculated voltage of the motor
     * in the interval [-255,+255], according to the currently active
     * controller of the board.
     */
    int calculate_voltage()
    {
        switch (controller) {
        case CONTROLLER_DISABLED:
            return 0;
            break;

        case CONTROLLER_PID:
            return float_to_int8(pid_control.step(get_position()));

        default:
            break;
        }
        return 0;
    }

    bool is_valid()
    {
        return (_id <= 127);
    }
};

#endif /* __BOARD_H */
