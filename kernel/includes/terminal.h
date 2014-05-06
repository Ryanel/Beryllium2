//Low Level Display Drivers
#ifndef TERMINAL_H
#define TERMINAL_H
#include <types.h>
#define STATUS_BAR_ATTRIBUTE 0x1F
///Prints a character
void printc(unsigned char c);
///Prints a string
void print(const char *c);
///Scrolls if needed
void scroll();
///Initialises the terminal
void terminal_init();
///Clears the statusbar
void terminal_clear_statusbar();
///Sets the status bar's text to c
void terminal_set_statusbar(const char *c);
#endif
