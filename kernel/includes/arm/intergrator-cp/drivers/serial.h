#ifndef SERIAL_H
#define SERIAL_H
#define SERIAL_BASE 0x16000000
#define SERIAL_FLAG_REGISTER 0x18
#define SERIAL_BUFFER_FULL (1 << 5)

#define UART0_ADDR 0x16000000

#define UART_FR_RXFE 0x10
#define UART_FR_TXFF 0x20

#define UART_DR(baseaddr) (*(unsigned int *)(baseaddr))
#define UART_FR(baseaddr) (*(((unsigned int *)(baseaddr))+6))

void serial_write(char c);
void serial_writes(char * s);
void serial_print_header();
#endif