#ifndef INTERRUPT_H
#define INTERRUPT_H
/**
A message to the interrupt system; describing a type of generic interrupt.

Contains a type, if it is queueable or should be handled immediately, and a pointer to data of size data_size
**/
typedef struct {
	int type;
	int isQueueable;
	void* data;
	int data_size;
	int isHandled;
} interrupt_message_t;

//TYPES
///A timer
#define IO_TYPE_TIMER 0
///The IO system
#define IO_TYPE_IO 0x1
///An exception
#define IO_TYPE_EXCEPTION 0x2
///Send a message to the IO system
void io_interrupt_recieve(interrupt_message_t *data);
///Cycle through the interrupt queue
void io_interrupt_docyclic();
///Handle a interrupt
void io_interrupt_handle(interrupt_message_t *data);
///Handle a dangerous interrupt
void io_interrupt_handle_dangerous(interrupt_message_t *data);
///Have we recieved an interrupt yet?
int isInterruptWorking();
#endif
