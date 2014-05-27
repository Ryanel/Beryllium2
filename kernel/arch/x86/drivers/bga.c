#include <x86/ports.h>
#include <x86/drivers/bga.h>
#include <beryllium/drivers/framebuffer.h>
#include <stdio.h>
#include <beryllium/driver.h>
#include <beryllium/device.h>
bool 		bga_isinit = false;
driver_t	bga_driver;

framebuffer_t bga_getFramebuffer()
{
	framebuffer_t builder;
	builder.memory_start = (uint8_t *)0xE0000000;
	builder.width = 800;
	builder.height = 600;
	builder.depth = 32;
	builder.pitch = 3200;
	builder.pixwidth = 4;
	return builder;
}

void bga_writeregister(uint16_t index, uint16_t value)
{
	outw(BGA_VBE_DISPI_IOPORT_INDEX, index);
	outw(BGA_VBE_DISPI_IOPORT_DATA, value);
}
uint16_t bga_readregister(uint16_t index)
{		
	outw(BGA_VBE_DISPI_IOPORT_INDEX, index);
    return inw(BGA_VBE_DISPI_IOPORT_DATA);
}

int bga_isavalable()
{
	uint16_t ver = bga_readregister(BGA_VBE_DISPI_INDEX_ID);
	if(ver < 0xB0C2)
	{
		return 0;
	}
	if(ver > 0xB0C5)
	{
		return 0;
	}
	return 1;
}

int bga_start()
{
	if(!bga_isavalable())
	{
		return 1;
	}
	bga_setmode(800,600,32,1,0);
	bga_isinit = true;
	return 0;
}

int bga_init()
{
	bga_driver.start = &bga_start;
	driver_start(&bga_driver);
	return 0;
}

void bga_setmode(unsigned int width, unsigned int height, unsigned int bitdepth, int linearframebuffer, int clearvideomemory)
{
	bga_writeregister(BGA_VBE_DISPI_INDEX_ENABLE, BGA_VBE_DISPI_DISABLED);
	bga_writeregister(BGA_VBE_DISPI_INDEX_XRES, width);
	bga_writeregister(BGA_VBE_DISPI_INDEX_YRES, height);
	bga_writeregister(BGA_VBE_DISPI_INDEX_BPP, bitdepth);
	bga_writeregister(BGA_VBE_DISPI_INDEX_ENABLE, BGA_VBE_DISPI_ENABLED |
	(linearframebuffer ? BGA_VBE_DISPI_LFB_ENABLED : 0) |
	(clearvideomemory ? 0 : BGA_VBE_DISPI_NOCLEARMEM));
}
