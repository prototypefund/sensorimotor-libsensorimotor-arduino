#include "../src/PID.h"
#include <cstdlib>
#include <stdio.h>

using std::abs;

struct pid_test {
    float p;
    float i;
    float d;
    float target;
    float input;
    float expected;
};

int test_pid()
{
    pid_test test[] = {
        { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
    };

    int len = (sizeof(test) / sizeof(*test));

    float output = 0.f;
    pid_test t;

    for (uint8_t i = 0; i < len; i++) {
        PID pid(0, 1.f / 100);

        t = test[i];

        pid.set_pid(t.p, t.i, t.d);

        pid.set_target_value(t.target);

        output = pid.step(t.input);

        if (output != t.expected) {
            printf("Error in test case %d\n", i + 1);
            printf("Value for %f was %f (expected %f)\n", t.input, output, t.expected);
            return i + 1;
        }
    }

    return 0;
}

int main(void)

{
    if (test_pid() != 0)
        return 1;
    return 0;
}