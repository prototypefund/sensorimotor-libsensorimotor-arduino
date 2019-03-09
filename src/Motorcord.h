#include <stdint.h>

#ifndef __MOTORCORD_H
#define __MOTORCORD_H

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

    // set prescaler to 64 (011)
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
    uint8_t last_timeslot;
    communication com;

public:
    Motorcord(){};
    ~Motorcord(){};
    // boards contains information of up to 10 connected boards
    Board boards[10];

    void init()
    {
        // TODO: FIXME: prime motors w/o discovery for now
        boards[0] = Board(0);
        boards[1] = Board(1);
        boards[2] = Board(2);
        boards[3] = Board(3);
    }

    void apply()
    {
        // keep track of timeslot as every millisecond we will increment the timeslot
        if (current_timeslot != last_timeslot) {
            // reset communication_state
            com.reset();
            current_timeslot = current_timeslot;
        }

        com.recv();

        if (com.syncstate == command_state_t::finished) {
        }
    }
};

#endif /* __MOTORCORD_H */