#include "Data.h"
#include <stdint.h>

#ifndef __BOARD_H
#define __BOARD_H

// TODO: where to put this?
const int motors_update_rate_hz = 100;

class Board {

    // id of the board, 0-127
    uint8_t _id;

    // embedded data about position, voltage, current etc.
    Data data;

    // PID specific
    PID pid_control;
    bool use_pid;
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

    Data get_data(Data d) { return data; };

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
