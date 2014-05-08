#include <types.h>
#include <log.h>
#include <stdio.h>
uint32_t initrd_exists = 0;
uint32_t initrd_startaddr = 0;
uint32_t initrd_endaddr = 0;

void initrd_set_location(uint32_t start, uint32_t end)
{
	initrd_startaddr	= start;
	initrd_endaddr		= end;
	initrd_exists		= 1;
}

int initrd_verify(uint32_t * addr)
{
	if(addr[0]=='I')
	{
		return 1;
	}
}

void initrd_init()
{
	if(!initrd_exists)
	{
		klog(LOG_PANIC,"initrd_init","No initrd? This won't end well\n");
		return;
	}
	if(initrd_verify((uint32_t*)initrd_startaddr))
	{
		klog(LOG_PANIC,"initrd_init","Valid Initrd Found\n");
	}
}