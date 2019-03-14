#include <Sensorimotor.h>

Motorcord motors;

void setup()
{
    motors.init();

    // you have to set a voltage to activate the motors.
    // voltage controls the PWM duty cycle. values range from
    // 0 – 1.0, where 1.0 is 100% duty cycle or the maximum voltage.
    // If your voltage is 12V, 0.5 would correspond to 6V max,
    // 0.25 to 3V max, etc.
    motors.set_max_voltage(0.15);

    // set the PID parameters and therefore enable the PID mode of the board.
    motors.get(0)->set_pid_values(1.f, 0.f, 0.f);
}

void loop()
{
    motors.apply();

    // you can now see the currently reported position by calling
    //     motors.get(0)->get_position();
    // to get a float from -1 (minimal pos) to +1 (maximal pos).

    // we may now also set a new desired position
    motors.get(0)->set_position(0.25);
}
