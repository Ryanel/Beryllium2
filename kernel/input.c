#include <log.h>
volatile unsigned char ibuffer[0xFF];
volatile unsigned int ibuffer_i = 0;

void input_add_char(char c)
{
	if(ibuffer_i == 0xFF)
	{
		klog(LOG_ERROR,"isys","Too much data! Dropping\n");
	}
	if(c == 0)
	{
		return;
	}
	ibuffer[ibuffer_i] = c;
	ibuffer_i++;
}

char input_read()
{
	if(ibuffer[0] == 0)
	{
		return 0;
	}
	char to_send = ibuffer[0];
	ibuffer[0] = 0;
	int i = 0;
	while(i<0xFF)
	{
		ibuffer[i] = ibuffer[i+1];
		i++;
	}
	ibuffer_i--;
	
	return to_send;
}