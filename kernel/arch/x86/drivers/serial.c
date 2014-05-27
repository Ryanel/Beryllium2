#include <x86/drivers/serial.h>
#include <x86/ports.h>
#include <stdio.h>
#include <beryllium/driver.h>
#include <beryllium/device.h>
driver_t			 serial_driver;
device_t             serial_device;
int serial_start()
{
	outb(COM1 + 1, 0x00);    // Disable all interrupts
	outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(COM1 + 1, 0x00);    //                  (hi byte)
	outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	return 0;
}

int serial_received() {
	return inb(COM1 + 5) & 1;
}

char serial_read() {
	while (serial_received() == 0);

	return inb(COM1);
}

char serial_read_nowait() {
	if(serial_received() == 1)
	{
		return inb(COM1);
	}
	return 0;
	
}
int serial_is_transmit_empty() {
	return inb(COM1 + 5) & 0x20;
}

void serial_write(char a) {
	while (serial_is_transmit_empty() == 0);
	outb(COM1,a);
}

void serial_print(const char *c)
{
	int i = 0;
	while (c[i])
	{
		serial_write(c[i++]);
	}
}
void serial_init() {
	serial_driver.start = &serial_start;
	serial_device.name       = "serial0";
	serial_device.type       = DEVICE_TYPE_HARDWARE;
	serial_device.flags      = 0;
	serial_device.interface  = DEVICE_INTERFACE_IO;
	serial_device.driver     = &serial_driver;
	device_start(&serial_device);
}
