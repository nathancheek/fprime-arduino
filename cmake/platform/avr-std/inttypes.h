/* Wraps avr-libc's <inttypes.h>, which omits the 64-bit format macros because its printf cannot format 64-bit
 * integers. Define them so code using them compiles; such values will not print correctly. */
#ifndef AVR_STD_INTTYPES_H
#define AVR_STD_INTTYPES_H
#include_next <inttypes.h>

#ifndef PRId64
#define PRId64 "lld"
#define PRIi64 "lli"
#define PRIo64 "llo"
#define PRIu64 "llu"
#define PRIx64 "llx"
#define PRIX64 "llX"
#endif
#endif
