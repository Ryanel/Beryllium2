#include <x86/low/isr.h>
#include <x86/low/idt.h>
#include <x86/low/irq.h>
#include <x86/ports.h>
#include <beryllium/timer.h>
#include <beryllium/driver.h>
#include <string.h>
#include <stdio.h>
#include <beryllium/device.h>
unsigned long        timer_ticks     = 0;
unsigned long        timer_ticks_old = 0;

driver_t			 pit_driver;
device_t             pit_device;
void pit_phase(int hz)
{
	int divisor = 1193180 / hz;       /* Calculate our divisor */
	outb(0x43, 0x36);             /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handler(struct regs *r)
{
	if(r->int_no) {} // Used to disable unused argument warning
	timer_ticks++;
	timer_recieveTick(0);
}

int pit_start()
{
	register_interrupt_handler(IRQ0,&timer_handler);
	pit_phase(1000);
	return 0;
}

int pit_stop()
{
	return 0xFFFFFFFF;
}

void pit_init()
{
	pit_driver.start = &pit_start;
	pit_driver.stop = &pit_stop;
	pit_device.name       = "pit";
	pit_device.type       = DEVICE_TYPE_HARDWARE;
	pit_device.flags      = 0;
	pit_device.interface  = DEVICE_INTERFACE_IO;
	pit_device.driver     = &pit_driver;
	device_start(&pit_device);
}
