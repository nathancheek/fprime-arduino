/* Wraps avr-libc's <stdio.h>, adding the vsscanf() it lacks. Built on a read-only fdev stream over the string, using
 * only the public fdev_* interface. */
#ifndef AVR_STD_STDIO_H
#define AVR_STD_STDIO_H
#include_next <stdio.h>

static inline int avr_std_string_getc(FILE* stream) {
    const char** cursor = (const char**)fdev_get_udata(stream);
    const char c = **cursor;
    if (c == '\0') {
        return _FDEV_EOF;
    }
    (*cursor)++;
    return (unsigned char)c;
}

static inline int vsscanf(const char* str, const char* fmt, va_list ap) {
    FILE stream;
    const char* cursor = str;
    fdev_setup_stream(&stream, NULL, avr_std_string_getc, _FDEV_SETUP_READ);
    fdev_set_udata(&stream, (void*)&cursor);
    return vfscanf(&stream, fmt, ap);
}
#endif
