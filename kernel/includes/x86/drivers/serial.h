#ifndef DRVSERIAL_H
#define DRVSERIAL_H
///Port number of COM1
#define COM1 0x3f8
///Initialises the serial
void serial_init();
///Have we gotten anything?
int serial_received();
///Read one character
char serial_read();
///Read one character but DONT wait for one if there is no character ready
char serial_read_nowait() ;
///Can we write?
int serial_is_transmit_empty();
///Write to COM1
void serial_write(char a);
///Prints a string to COM1
void serial_print(const char *c);
#endif
