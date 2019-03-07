#include "pid2.hpp"

#ifndef __SENSORIMOTOR_H
#define __SENSORIMOTOR_H




inline float uint16_to_sc(uint16_t val) {
  return (val - 32768) / 32768.f;
}
inline float  int16_to_sc(uint16_t val) {
  return (int16_t) val / 32768.f;
}

volatile uint8_t current_timeslot = 0;
ISR (TIMER1_COMPA_vect)
{
  // iterate through timeslot 0-9
  current_timeslot++;
  current_timeslot %= 10;
}

void init_timers(void) {
  TCCR1A = 0; // Reset timer 1 control register A

  TCNT1 = 0; // intial timer value
  OCR1A = 250; // compare register

  // set prescaler to 64 (011)
  TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
  TCCR1B |= (1 << CS10);

  // set CTC mode (reset timer automatically on interrupt)
  TCCR1B &= ~(1 << WGM13); // clear bit if set
  TCCR1B |= (1 << WGM12); // set WGM12

  // enable compare interrupt
  TIMSK1 = (1 << OCIE1A);
}

void main_loop(void) {
  // keep track of timeslot; every millisecond we will increment the timeslot
  uint8_t previous_timeslot = 0;
  unsigned long now = millis();
  while (true) {
    if (current_timeslot != previous_timeslot) {
      // reset communication_state


      previous_timeslot = current_timeslot;
    }

  }
}

class sensorimotor {

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
    sensorimotor(uint8_t id) : _id(id) {};
    sensorimotor() : _id(0xFF) {}; // id = uninitialized
    ~sensorimotor() {};

    uint8_t ID() {
      return _id;
    }

    bool is_valid() {
      return (_id <= 127);
    }

};

class Motorcord {
  private:
    uint8_t last_timeslot;
    communication com;
  public:
    Motorcord() {};
    ~Motorcord() {};
    // motors contains information of up to 10 connected motors
    sensorimotor motors[10];

    void init() {
      // TODO: FIXME: prime motors w/o discovery for now
      motors[0] = sensorimotor(0);
      motors[1] = sensorimotor(1);
      motors[2] = sensorimotor(2);
      motors[3] = sensorimotor(3);
    }

    void apply() {
      if (current_timeslot != last_timeslot) {
        // reset communication_state
        com.reset();
        current_timeslot = current_timeslot;
      }

      com.recv();

      if (com.syncstate == communication::command_state_t::finished) {

      }
    }
};

#endif /* __SENSORIMOTOR_H */
