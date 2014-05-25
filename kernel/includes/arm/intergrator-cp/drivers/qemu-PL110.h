#ifndef PL110_H
#include <types.h>
#define PL110_H
#define PL110_CR_EN			0x001
#define PL110_CR_PWR		0x800
#define PL110_IOBASE		0xc0000000
#define PL110_PALBASE		(PL110_IOBASE + 0x200)
typedef struct _PL110MMIO 
{
	uint32_t	volatile tim0;		//0
	uint32_t	volatile tim1;		//4
	uint32_t	volatile tim2;		//8
	uint32_t	volatile d;		//c
	uint32_t	volatile upbase;	//10
	uint32_t	volatile f;		//14
	uint32_t	volatile g;		//18
	uint32_t	volatile control;	//1c
} pl110_mmio_t;
void qemu_pl110_write_index(uint32_t index,uint8_t r,uint8_t g,uint8_t b);
void qemu_pl110_write(int x, int y,uint8_t r,uint8_t g,uint8_t b);
#endif