#include <arm/intergrator-cp/drivers/serial.h>
void serial_write(char c)
{
	/* Wait until the serial buffer is empty */
	while (*(volatile unsigned long*)(SERIAL_BASE + SERIAL_FLAG_REGISTER) & (SERIAL_BUFFER_FULL));
	{

	}
	*(volatile unsigned long*)SERIAL_BASE = c;
	if(c == '\n')
	{
		serial_write('\r');
	}
}
void serial_writes(char * s)
{
	while (*s) serial_write(*s++);
}