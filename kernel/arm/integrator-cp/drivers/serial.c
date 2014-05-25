#include <arm/intergrator-cp/drivers/serial.h>
#include <arm/mmio.h>
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

char serial_read()
{
	if(!(UART_FR(UART0_ADDR) & UART_FR_RXFE))
	{
		return UART_DR(UART0_ADDR);
	}
	return 0x0;
}

void serial_init()
{
	serial_read();
}