#include "Data.h"
#include <stdint.h>

#ifndef __MOTORCORD_H
#define __MOTORCORD_H

const uint8_t MAX_BOARDS = 10;

/*
 * current_timeslot  keeps track of the current timeslot; This global
 * variable will increment once every millisecond, which will
 * prompt the communication loop to clear the current state and handle
 * requests for other boards.
 */
volatile uint8_t current_timeslot = 0;

/* Interrupt service routine macro to automatically increment timeslot */
ISR(TIMER1_COMPA_vect)
{
    // iterate through timeslot 0-9
    current_timeslot++;
    current_timeslot %= 10;
}

/*
 * init_timers will initialize the timers and interrupts required for
 * the communication with the motor cord.
 */
void init_timers(void)
{
    TCCR1A = 0; // Reset timer 1 control register A

    TCNT1 = 0; // intial timer value
    OCR1A = 250; // compare register

    // set prescaler to 64 (b011)
    TCCR1B &= ~(1 << CS12);
    TCCR1B |= (1 << CS11);
    TCCR1B |= (1 << CS10);

    // set CTC mode (reset timer automatically on interrupt)
    TCCR1B &= ~(1 << WGM13); // clear bit, if set
    TCCR1B |= (1 << WGM12); // set WGM12

    // enable compare interrupt
    TIMSK1 = (1 << OCIE1A);
}

class Motorcord {
private:
    uint8_t last_timeslot = 0;

    Communication com;

    void _parse_status()
    {

        boards[0].dbgSuccess = com.syncstate;

        // abort if message is not ready yet
        if (com.syncstate != command_state_t::finished) {
            return;
        };

        // abort if message is not a data response
        if (com.get_message_type() != command_t::CMD_RESPONSE_DATA) {
            return;
        }

        // discard sync bytes
        com.pop_word();

        uint8_t board_id = com.pop();
        if (board_id > MAX_BOARDS)
            // would write outside of array bounds,
            // we cannot handle more than MAX_BOARDS
            // yet.
            return;

        // TODO: this is communication related, move to Communication
        // the order of this commands is important, based on protocol!
        Data data;
        data.raw_pos = com.pop_word();
        data.raw_current = com.pop_word();
        data.raw_velocity = com.pop_word();
        data.raw_voltage = com.pop_word();
        data.raw_temp = com.pop_word();

        // assign data to board
        boards[board_id].set_data(data);

        // TODO: parse data message better

        return;
    };

public:
    Motorcord(){};
    ~Motorcord(){};
    // boards contains information of up to 10 connected boards
    Board boards[MAX_BOARDS];

    void init()
    {
        init_timers();

        // TODO: FIXME: prime motors w/o discovery for now
        boards[0] = Board(0);
        boards[1] = Board(1);
        boards[2] = Board(2);
        boards[3] = Board(3);
        boards[4] = Board(4);
        boards[5] = Board(5);
        boards[6] = Board(6);
        boards[7] = Board(7);
        boards[8] = Board(8);
        boards[9] = Board(9);
    };

    /* `apply` is the main motor control routine. It is designed to be
     * called multiple times per millisecond.
     * It works the following way:
     * 1) Receive new incoming bytes from serial buffer
     * 2) Parse response from read bytes, after the timeslot has ended
     * 3) On new timeslot, send control requests to next motor.
     */
    void apply()
    {
        // read everything we have received so far
        com.recv();

        // keep track of timeslot as every millisecond we will increment the timeslot
        if (current_timeslot != last_timeslot) {
            // read everything we have received so far
            _parse_status();

            // we are dealing with board [current_timeslot] now.

            // reset communication_state, it's the beginning of a new era
            com.reset();
            last_timeslot = current_timeslot;

            // Set voltage according to controller, this will also prompt a
            // data response which can be evaluated in the next timestep.
            if (get(current_timeslot)->is_valid()) {
                uint16_t calculated_voltage = get(current_timeslot)->calculate_voltage();
                com.send_set_voltage_request(current_timeslot, calculated_voltage);
            }
        }
    }

    void stop()
    {
        // for each motor in cord
        for (uint8_t i = 0; i < MAX_BOARDS; i++) {
            boards[i].set_controller(CONTROLLER_DISABLED);
        }
        return;
    };

    void set_max_voltage(float pwm)
    {
        // for each motor in cord
        for (uint8_t i = 0; i < MAX_BOARDS; i++) {
            com.send_pwm_limit_request(i, float_to_uint8(pwm));
            delayMicroseconds(75);
        }
        return;
    };

    Board* get(int id)
    {
        return &boards[id];
    };
};

#endif /* __MOTORCORD_H */
