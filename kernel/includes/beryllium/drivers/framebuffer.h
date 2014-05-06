#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <types.h>
/**
	Defines a framebuffer
**/
typedef struct {
	uint8_t *memory_start;
	uint8_t *memory_end; //Does not have to be assigned.
	int width;
	int height;
	int depth;
	int pitch;
	int pixwidth;

} framebuffer_t;
/**
	Puts a pixel into framebuffer fb
**/
void fb_putpixel(framebuffer_t fb, int x,int y, int color);

#endif
