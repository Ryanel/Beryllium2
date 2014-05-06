#include <types.h>
#include <log.h>
#include <beryllium/device.h>
#include <beryllium/watchdog.h>
#include <string.h>
#include <beryllium/timing.h>
#include <beryllium/timer.h>
#include <stdio.h>
timer_t handlers[0xFF];//We assume that we will never need more than 0xFF, but we can change that in the future to rely on malloc

int timing_register_timer(char *name,int tick,void (*handler)(timer_t *), int reschedule)
{
	int index = 0;
	for(index = 0;index<0xFF;index++)
	{

		if(handlers[index].fire_tick == 0)
		{
			break;
		}
		else
		{
			continue;
		}
	}
	if(index == 0xFF)
	{
		klog(LOG_ERROR,"Failed to register timer %s!\n",name);
		return 0xFF;
	}
	timer_t builder;
	builder.fire_tick = tick + timer_getHi();
	builder.name = name;
	builder.repeat_rate = reschedule;
	builder.handler = handler;
	builder.no = index;
	handlers[index] = builder;
	return index;

}

void timing_deregister_timer(void (*handler)(timer_t *))
{
	for(int index = 0;index<0xFF;index++)
	{

		if(handlers[index].handler == 0)
		{
			continue;
		}
		else
		{
			if(handlers[index].handler == handler)
			{
				handlers[index].handler = 0;
				handlers[index].repeat_rate = 0;
				handlers[index].fire_tick = 0;
				break;
			}
			continue;
		}
	}
}

void timing_remove_index(int index)
{
	handlers[index].repeat_rate = 0;
}

void list_timers()
{
	printf(" ID |%-11sNAME|NEXT    ms|REP ms\n","",0,0);
	int index = 0;
	int current_time = timer_getHi();
	char *unit = "ms";
	for(index = 0;index<0xFF;index++)
	{
		if(handlers[index].fire_tick == 0)
		{
			continue;
		}
		else
		{
			int rep_in = handlers[index].fire_tick - current_time;
			if(rep_in > 1000)
			{
				unit = "s";
				rep_in = rep_in / 1000;
			}
			else
			{
				unit = "ms";
			}
			printf("[%02x]:%-15s|%8d%2s|%-04dms\n",index,handlers[index].name,rep_in,unit,handlers[index].repeat_rate);
		}
	}
}

void list_timers_dbg()
{
	
}

void timing_reaper()
{
	int t_i = 0;
	int tick = timer_getHi();
	for (t_i = 0; t_i < 0xFF; t_i++)
	{
		if(handlers[t_i].fire_tick == 0)
		{
			continue;
		}
		if(handlers[t_i].fire_tick < tick)
		{
			handlers[t_i].fire_tick = 0;
			handlers[t_i].repeat_rate = 0;
			handlers[t_i].handler = 0;
		}
	}
}

int timing_init()
{
	memset(&handlers,0,sizeof(timer_t) * 0xFF);
	timing_register_timer("timer_reaper",100,timing_reaper, 100);
	return 0;
}
int t_i = 0;
int cyclic_tasks(int tick)
{
	t_i = 0;
	for (t_i = 0; t_i < 0xFF; t_i++)
	{
		if(handlers[t_i].fire_tick == 0)
		{
			continue;
		}
		if(handlers[t_i].fire_tick == tick)
		{
			handlers[t_i].handler(&handlers[t_i]);
			if(handlers[t_i].repeat_rate != 0)
			{
				handlers[t_i].fire_tick = tick + handlers[t_i].repeat_rate;
			}
		}
	}
	return 0;
}