#include <beryllium/drivers/framebuffer.h>
void fb_putpixel(framebuffer_t fb, int x,int y, int color) {
	unsigned where = x*fb.pixwidth + y*fb.pitch;
	fb.memory_start[where] = color & 255;              // BLUE
	fb.memory_start[where + 1] = (color >> 8) & 255;   // GREEN
	fb.memory_start[where + 2] = (color >> 16) & 255;  // RED
}
