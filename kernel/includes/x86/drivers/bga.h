#ifndef BGA_H
#define BGA_H

#include <types.h>
#include <beryllium/drivers/framebuffer.h>

#define BGA_VBE_DISPI_IOPORT_INDEX 0x01CE
#define BGA_VBE_DISPI_IOPORT_DATA 0x01CF
#define BGA_VBE_DISPI_INDEX_ID 0
#define BGA_VBE_DISPI_INDEX_XRES 1
#define BGA_VBE_DISPI_INDEX_YRES 2
#define BGA_VBE_DISPI_INDEX_BPP 3
#define BGA_VBE_DISPI_INDEX_ENABLE 4
#define BGA_VBE_DISPI_INDEX_BANK 5
#define BGA_VBE_DISPI_INDEX_VIRT_WIDTH 6
#define BGA_VBE_DISPI_INDEX_VIRT_HEIGHT 7
#define BGA_VBE_DISPI_INDEX_X_OFFSET 8
#define BGA_VBE_DISPI_INDEX_Y_OFFSET 9

#define BGA_VBE_DISPI_DISABLED              0x00
#define BGA_VBE_DISPI_ENABLED               0x01
#define BGA_VBE_DISPI_GETCAPS               0x02
#define BGA_VBE_DISPI_8BIT_DAC              0x20
#define BGA_VBE_DISPI_LFB_ENABLED           0x40
#define BGA_VBE_DISPI_NOCLEARMEM            0x80
///Write a value to the bga
void bga_writeregister(uint16_t index, uint16_t value);
///Reads a value from the bga
uint16_t bga_readregister(uint16_t index);
///Sets video mode
void bga_setmode(unsigned int width, unsigned int height, unsigned int bitdepth, int linearframebuffer, int clearvideomemory);
///Can we use the BGA?
int bga_isavalable();
///Initialises the BGA
int bga_init();
///Gets the Framebuffer
framebuffer_t bga_getFramebuffer();
#endif