#ifndef STDIO_H
#define STDIO_H
#include <stdarg.h>
/// wikipedia.org/printf
/**
Prints formated text. Can print to any video device implemented in video.c
**/
int printf(const char *fmt, ...);
int serial_printf(const char *fmt, ...);
int printf_at(int x, int y,const char *fmt, ...);
int printf_at_cc(int x, int y,unsigned char color,const char *fmt, ...);
/**
Prints formated text.
**/
int vprintf(const char *fmt, va_list args);
int vprintf_serial(const char *fmt, va_list args);
/**
Writes formatted data to string
**/
void sprintf(char *buf, const char *fmt, ...);
#endif
