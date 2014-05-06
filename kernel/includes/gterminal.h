//Low Level Display Drivers
#ifndef GTERMINAL_H
#define GTERMINAL_H
#include <types.h>
///Prints a character
void gterminal_printc(unsigned char c);
///Prints a string
void gprint(const char *c);
///Scrolls if needed
void gscroll();
#endif
