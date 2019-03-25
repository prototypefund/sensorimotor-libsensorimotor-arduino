#ifndef ARDUINO
// required header for printf()
#include <cstdio>
#endif /* ARDUINO */

#ifndef __ASSERT_H
#define __ASSERT_H

#ifdef ARDUINO
#define assert(condition)                         \
    if (!condition)                               \
        for (;;) {                                \
            Serial.print("Assert error: File '"); \
            Serial.print(F(__FILE__));            \
            Serial.print(":");                    \
            Serial.print(__LINE__, 10);           \
            Serial.print("'\n");                  \
            delay(1500);                          \
        }
#else // not arduino
#define assert(condition) __assert(condition, __FILE__, __LINE__)
void __assert(bool condition, const char* file, const int line)
{
    if (condition) {
        return;
    }

    printf("Assert error: File '%s:%d'\n");
}
#endif

#endif /* __ASSERT_H */