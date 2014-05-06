#include <log.h>
#include <types.h>
#include <beryllium/timer.h>
#include <beryllium/ktaskman.h>
/** High resolution timer ticks. Set every single clock tick **/
uint32_t timer_hi = 0;
/** Low resolution timer ticks/ Set every single overflow of timer_hi **/
uint32_t timer_low = 0;
uint32_t device = 0; //0 - pit, 1 - rtc. Used as the source. Will be replaced by a driver_t;
uint32_t device_resoulution = 1; //How many ticks make a millisecond

void timer_switchDevice(uint32_t device_id)
{
	switch (device_id)
	{
		case 0x0:
			device = 0;
			device_resoulution = 1;
			break;
		case 0x2:
			device = 2;
			device_resoulution = 1;
			break;
		default:
			timer_switchDevice(0);
			break;
	}
}

void timer_recieveTick(uint32_t device_id)
{
	if(device_id==device)
	{
		timer_hi++;
		if((timer_hi % (device_resoulution * 1000)) == 0)
		{
			//klog(LOG_DEBUG,"Timer","Device %d (%s), resolution = %d (tick no %d)\n",device_id,timer_getDeviceString(device_id),device_resoulution,timer_hi);
			timer_low++;
		}
		cyclic_tasks(timer_hi);
	}
}

uint32_t timer_getHi()
{
	return timer_hi;
}

uint32_t timer_getDevice()
{
	return device;
}	

const char* timer_getDeviceString(uint32_t device)
{
	switch(device)
	{
		case 0x0:
			return "Programmable interrupt timer";
		case 0x1:
			return "Real time clock";
		case 0x2:
			return "ARM Integrated";
		default:
			return "Unknown";
	}
}

uint32_t timer_getDeviceRes()
{
	return device_resoulution;
}

uint32_t timer_getLow()
{
	return timer_low;
}
#ifdef X86
uint32_t timer_getUptime()
{
	return timer_hi/(device_resoulution*1000);
}
#endif
