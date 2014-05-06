#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <beryllium/driver.h>
#include <log.h>

int driver_start(driver_t * driver)
{
	if(driver->start)
	{
		int ret = driver->start();
		return ret;
	}
	else
	{
		klog(LOG_ERROR,"DRV","Driver does not support starting!\n");
		return 0xFFFFFFFF;
	}
}
