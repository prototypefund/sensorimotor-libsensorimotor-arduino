#include "Data.h"
#include "utils.h"
#include <stdint.h>

#ifndef __BOARD_H
#define __BOARD_H

// TODO: where to put this?
const int motors_update_rate_hz = 100;

enum controller_t : uint8_t {
    CONTROLLER_DISABLED,
    CONTROLLER_VOLTAGE,
    CONTROLLER_PID,
    CONTROLLER_CONSTP,
    CONTROLLER_SPEED,
    CONTROLLER_IMPULSE
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

    uint8_t ID()
    {
        return _id;
    };

    bool is_valid()
    {
        return (_id <= 127);
    };

    /* get_position is a shorthand for get_data().raw_pos */
    float get_position()
    {
        return data.get_position();
    };

    /* get_current is a shorthand for get_data().get_current() */
    float get_current() { return data.get_current(); };

    /* get_velocity is a shorthand for get_data().raw_velocity */
    int get_velocity() { return data.raw_velocity; };

    /* get_voltage is a shorthand for get_data().get_voltage */
    float get_voltage() { return data.get_voltage(); };

    /* get_temp is a shorthand for get_data().get_temp() */
    float get_temp() { return data.get_temp(); };

    /*
     * `calculate_voltage` returns the calculated voltage of the motor
     * in the interval [-255,+255], according to the currently active
     * controller of the board.
     */
    uint16_t calculate_voltage()
    {
        switch (controller) {
        case CONTROLLER_DISABLED:
            return 0;
            break;

        case CONTROLLER_PID:
            float u = pid_control.step(get_position());
            return float_to_int16(u);

        default:
            break;
        }
        return 0;
    };

    Data get_data() { return data; };

    void set_data(Data d) { data = d; };

    void set_controller(controller_t controller_type) { controller = controller_type; };

    void set_pid_values(float p, float i, float d)
    {
        pid_control.set_pid(p, i, d);
        set_controller(CONTROLLER_PID);
    };

    void set_position(float position)
    {
        pid_control.set_target_value(position);
        set_controller(CONTROLLER_PID);
    };
};

#endif /* __BOARD_H */
