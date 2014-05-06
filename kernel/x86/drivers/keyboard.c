#include <x86/low/isr.h>
#include <x86/low/idt.h>
#include <x86/low/irq.h>
#include <x86/ports.h>
#include <beryllium/timer.h>
#include <beryllium/driver.h>
#include <string.h>
#include <stdio.h>
#include <beryllium/device.h>
#include <mutex.h>
#define KB_TRAP_CTL    0x01
#define KB_TRAP_LSHIFT 0x02
#define KB_TRAP_RSHIFT 0x03
#define KB_TRAP_ALT    0x04
#define KB_CAPS        0x05
#define KB_F1          0x06

driver_t			 keyboard_driver;
device_t             keyboard_device;

volatile unsigned char buffer[0xFF];
volatile unsigned int buffer_i = 0;

mutex_t *kbio_mutex = 0;

//Taken shamelessly from Bran's kernel development tutorial
unsigned char kbdus[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	KB_TRAP_CTL,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   KB_TRAP_LSHIFT,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   KB_TRAP_RSHIFT,				/* Right shift */
  '*',
	KB_TRAP_ALT,	/* Alt */
  ' ',	/* Space bar */
	KB_CAPS,	/* Caps lock */
	KB_F1,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
  '-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
  '+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};		
unsigned char kbdus_caps[128] =
{
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',		/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
	KB_TRAP_CTL,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   KB_TRAP_LSHIFT,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   KB_TRAP_RSHIFT,				/* Right shift */
  '*',
	KB_TRAP_ALT,	/* Alt */
  ' ',	/* Space bar */
	KB_CAPS,	/* Caps lock */
	KB_F1,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
  '-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
  '+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};		

int kb_shift = 0;
int kb_caps = 0;
int kb_alt   = 0;
int kb_ctl   = 0;
int kb_extended = 0;
void kb_handler(struct regs *r)
{
	unsigned char scancode;
	scancode = inb(0x60);
	
    if(kb_extended)
    {
        kb_extended = 0;
        return;
    }
    if(scancode == 0xE0) //Extended keys
    {
        kb_extended = 1;
        return;
    }
	if(scancode == 0xAA)
	{
		kb_shift = 0;
		return;
	}
	if(scancode == 0x2A)
	{
		kb_shift = 1;
		return;
	}
    if (scancode & 0x80)
    {
	    if(kbdus[scancode] == KB_TRAP_CTL)
		{
			kb_ctl = 1;
			return;
		}
		else if(kbdus[scancode] == KB_TRAP_LSHIFT)
		{
			kb_shift = 1;
			return;
		}
		else
		{
			return;
		}
    }
	
	if(buffer_i == 0xFF)
	{
		serial_printf("Kernel keyboard driver overloaded! Dropping character %c\n",kbdus[scancode]);
		return;
	}

	if(kb_shift || kb_caps)
	{
		buffer[buffer_i] = kbdus_caps[scancode];
	}
	else
	{
		buffer[buffer_i] = kbdus[scancode];
	}
	buffer_i++;
}

int kb_start()
{
	register_interrupt_handler(IRQ1,&kb_handler);
	int i = 0;
	while(i!=0xFF)
	{
		buffer[i] = 0;
		i++;
	}
	return 0;
}

int kb_stop()
{
	deregister_interrupt_handler(IRQ1);
	return 0x0;
}
void kb_init()
{
	keyboard_driver.start = &kb_start;
	keyboard_driver.stop = &kb_stop;
	keyboard_device.name       = "ps2_keyboard";
	keyboard_device.type       = DEVICE_TYPE_HARDWARE;
	keyboard_device.flags      = 0;
	keyboard_device.interface  = DEVICE_INTERFACE_IO;
	keyboard_device.driver     = &keyboard_driver;
	device_start(&keyboard_device);
}

unsigned char kb_read()
{
	if(buffer[0] == 0)
	{
		mutex_unlock(kbio_mutex);
		return 0; //If there are no keys pressed, return an error code
	}
	asm("cli");
	mutex_lock(kbio_mutex);
	unsigned char to_send = buffer[0];
	buffer[0] = 0;
	int i = 0;
	while(i<0xFF)
	{
		buffer[i] = buffer[i+1];
		i++;
	}
	buffer_i--;
	mutex_unlock(kbio_mutex);
	asm("sti");
	return to_send;
}
