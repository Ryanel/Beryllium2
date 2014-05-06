#ifndef VTERM_H
#define VTERM_H
#include <types.h>
typedef struct {
	int height;
	int width;
	int vwidth;
	int vheight;
	int isInit;
	int isPhysical;
	char* data; /// A memory region of size (height * width) bytes
} term_t;

/**
Creates a terminal object
Returns a pointer to a ready-to-use terminal
**/
term_t* term_create(int height, int width, int isPhysical);

void term_write(term_t *term,int x,int y,char c);
void term_writes(term_t *term,int x,int y,char *c);
char term_read(term_t *term,int x,int y);
#endif
