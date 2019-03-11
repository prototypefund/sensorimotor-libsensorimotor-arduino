#include <stdint.h>

#ifndef __PID_H
#define __PID_H

/* UTILS */

/* clips values to Interval [-1,+1] */
double clip(double x);

/* clips values to Interval [-ul_limit,+ul_limit] */
double clip(double x, double ul_limit);

/* clips values to Interval [l_limit, u_limit] */
double clip(double x, double l_limit, double u_limit);

/* signum */
double sign(double x);

/* generates a pseudo-random double between 0.0 and 0.999... */
double random_value(void);

double
sign(double x)
{
    if (x > 0.0)
        return 1.0;
    else if (x < 0.0)
        return -1.0;
    else
        return 0.0;
}

/* checks if variable is in the given range [lower, upper]*/
template <typename T>
inline bool in_range(T value, T lower, T upper)
{
    return (lower <= value) and (value <= upper);
}

/* generates a random value within interval [a,b] */
double
random_value(double a, double b)
{
    /* switch interval borders if necessary */
    double temp;
    if (b < a) {
        temp = a;
        a = b;
        b = temp;
    }
    /* generate random value for the given interval */
    return (b - a) * ((double)rand()) / RAND_MAX + a;
}

/* clips values to Interval [-1,+1] */
double clip(double x)
{
    if (x > 1.0)
        return 1.0;
    else if (x < -1.0)
        return -1.0;
    else
        return x;
}

/* clips values to Interval [-ul_limit,+ul_limit] */
double clip(double x, double ul_limit)
{
    if (x > ul_limit)
        return ul_limit;
    else if (x < -ul_limit)
        return -ul_limit;
    else
        return x;
}

/* clips values to Interval [l_limit, u_limit] */
double clip(double x, double l_limit, double u_limit)
{
    if (x > u_limit)
        return u_limit;
    else if (x < l_limit)
        return l_limit;
    else
        return x;
}

/** Low-frequency pulse anti-friction mode:
    If the output is too small to actually move the motor, use irregular pulses of duration dt and
    a minimum height of 'threshold'. These pulses make the motors overcome static friction.
    The probability is proportional to the duty cycle 'value/threshold',
    and is zero below a certain dead_band.
*/
float chop(float value, float threshold, float deadband)
{
    if (abs(value) < threshold) {
        const float prob = abs(value) / threshold;
        return (random_value(deadband, 1.f) < prob) ? threshold * sign(value) : 0.f;
    } else {
        return in_range(value, -deadband, deadband) ? 0.f : value;
    }
}

class PID {

    uint8_t id;
    /* `dt` is the timestep between two measurements (delta t) */
    float dt;

    /* error integral */
    float err_int = 0.f;
    /* last known error */
    float err_lst = 0.f;
    /* desired setpoint */
    float target_value = 0.f;

    float Kp = 0.f;
    float Ki = 0.f;
    float Kd = 0.f;

    /* maximum applied output value */
    float limit = 1.f;
    float pulse_mode_threshold = 0.f;
    float dead_band = 0.f;

public:
    PID(uint8_t id, float timestep)
        : id(id)
        , dt(timestep)
    {
        //dbg_msg("creating PID controller with time step: %1.4f", timestep);
    }

    float step(float current_value)
    {
        const float err = target_value - current_value;
        const float err_dif = err - err_lst;

        //dbg_msg("pos: %1.2f (%u)", current_value, id);
        /**TODO integral anti-windup */
        err_int += err;
        err_int = clip(err_int);

        err_lst = err;

        const float out = Kp * err + Ki * err_int * dt + Kd * err_dif / dt;

        return chop(clip(out, limit), pulse_mode_threshold, dead_band);
    }

    void set_pid(float p, float i, float d)
    {
        /*assert(p >= 0.f);*/ Kp = p;
        /*assert(i >= 0.f);*/ Ki = i;
        /*assert(d >= 0.f);*/ Kd = d;
    }

    void set_target_value(float t)
    {
        target_value = t;
    }
    void set_limit(float l)
    {
        //assert(l >= 0.f);
        limit = l;
    }
    void set_dead_band(float d)
    {
        //assert(d >= 0.f);
        dead_band = d;
    }
    void set_pulse_mode_threshold(float t)
    {
        //assert(t >= 0.f);
        pulse_mode_threshold = t;
    }

    void reset(void)
    {
        err_int = 0.f;
        err_lst = 0.f;
    }
};

#endif /* __PID_H */