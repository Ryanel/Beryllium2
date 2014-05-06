#ifndef SERIAL_H
#define SERIAL_H
#define SERIAL_BASE 0x16000000
#define SERIAL_FLAG_REGISTER 0x18
#define SERIAL_BUFFER_FULL (1 << 5)

void serial_write(char c);
void serial_writes(char * s);
void serial_print_header();
#endif