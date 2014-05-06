#include <stdio.h>
#include <beryllium/watchdog.h>
#include <error.h>
#include <beryllium/timing.h>
int wd_kmain 			= 0;
int wd_kmain_o 			= 0;
int wd_kmain_hangups 	= 0;
int wd_total_hangups 	= 0;

int wd_enabled = 1;

void wd_disable()
{
	wd_enabled = 0;
}
void wd_enable()
{
	wd_enabled = 1;
}
void wd_notify(int source)
{
	if(!wd_enabled)
	{
		return;
	}
	if(source == WD_NOTIFY_KMAIN)
	{
		wd_kmain++;
	}
}

void wd_evaluate(timer_t *value)
{
	if(wd_kmain == wd_kmain_o)
	{
		wd_total_hangups++;
		wd_kmain_hangups++;
		if(wd_kmain_hangups > 10) //Sets a limit for annoying messages
		{
			serial_printf("Watchdog:[kmain] (%04d/%04d) [kernel](%04d/%04d)\n",wd_kmain_hangups,WD_HU_LIMIT_KMAIN,wd_total_hangups,WD_HU_LIMIT);
		}
	}
	if(wd_total_hangups > WD_HU_LIMIT)
	{
		printf("Kernel hung up too much! It hung up %d times!\n",wd_total_hangups);
		panic("Kernel was terminated by watchdog\n");
	}
	else if (wd_kmain_hangups > WD_HU_LIMIT_KMAIN)
	{
		printf("kmain hung up too much! It hung up %d times\n",wd_kmain_hangups);
		panic("Kernel was terminated by watchdog!\n");
	}
	wd_kmain_o = wd_kmain;
}

int wd_get_kmain_hangups()
{
	return wd_kmain_hangups;
}
int wd_get_hangups()
{
	return wd_total_hangups;
}
void wd_init()
{
	timing_register_timer("kernel_wd",25,wd_evaluate, 25);
}
