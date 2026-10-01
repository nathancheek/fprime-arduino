/* Wraps avr-libc's <stdlib.h>, adding the C99 conversions it lacks. Long long conversions are limited to 32 bits and
 * report ERANGE beyond that; float and double are the same type on avr-gcc. */
#ifndef AVR_STD_STDLIB_H
#define AVR_STD_STDLIB_H
#include_next <stdlib.h>

static inline long long strtoll(const char* str, char** end, int base) {
    return strtol(str, end, base);
}
static inline unsigned long long strtoull(const char* str, char** end, int base) {
    return strtoul(str, end, base);
}
static inline float strtof(const char* str, char** end) {
    return strtod(str, end);
}
#endif
