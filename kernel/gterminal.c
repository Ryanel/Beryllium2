#include <terminal.h>
#include <types.h>
#include <beryllium/video.h>
#include <beryllium/video_font.h>
#ifdef X86
#include <x86/ports.h>
#endif
volatile uint32_t term_x = 0;
volatile uint32_t term_y = 0;

///
///Determines if the screen needs to be scrolled, and scrolls.
///

void gscroll()
{
	if(term_y >= 25)
	{
		video_scroll(0,24);
		term_y = 24;
	}
}

void gprintchar(unsigned char c, int x, int y)
{
	uint8_t row = 0;
	uint8_t col = x * 8;
	uint8_t ry  = y * 8;
	for (row = 0; row < 8; row++) {
		
		for (uint8_t i = 0; i < 8; i++) {
			if (font8x8_basic[c][row] & (1 << i)) 
			{
				video_draw_pixel(col+i,ry+row, 0xFF,0xFF,0xFF);
			}
		}
	}
	
}

///
///Better than textmode_write, it formats the output at a basic level.
///
void gprintc(unsigned char c)
{
	if (c == 0x08 && term_x) //Backspace
	{
		term_x--;
	}
	else if (c == 0x09) //Tab
	{
		term_x = (term_x+8) & ~(8-1);
	}
	else if (c == '\r') //Return
	{
	   term_x = 0;
	}

	else if (c == '\n') //Newline
	{
	   term_x = 0;
	   term_y++;
	}
	else if(c >= ' ') //Anything else
	{
		gprintchar(c, term_x, term_y);
		term_x++;
	}

	if (term_x >= 80)
	{
		term_x = 0;
		term_y ++;
	}

	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	video_setcursor(term_x,term_y);
}

///
///Prints a basic string
///
void gprint(const char *c)
{
	int i = 0;
	while (c[i])
	{
		gprintc(c[i++]);
	}
}
void gterminal_clear()
{
	video_clear();
}
///
///Initialises the whole thing
///

void gterminal_init()
{
	gterminal_clear();
}
