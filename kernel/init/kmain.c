#include <stdio.h>
#include <beryllium/video.h>
#include <terminal.h>
#include <log.h>
#include <beryllium/timer.h>
#include <system.h>
#include <stdlib.h>
#include <beryllium/vfs.h>
#include <beryllium/fs/device_man.h>
#include <beryllium/vterm.h>
#include <beryllium/thread.h>
#include <beryllium/driver.h>
#include <beryllium/watchdog.h>
#include <beryllium/kmonitor.h>
#include <beryllium/timing.h>
#include <beryllium/debug.h>
#include <error.h>

#ifdef X86
void x86_switch_to_usermode();
#endif

void init_vfs_devices();
void kshell_init();
void kshell_parse_char(char);
char kb_read();
void device_manager_start();

int total_shells = 4;
char * shells[4]={"/bin/init","/sbin/init","/usr/bin/init","/xbin/init"};

/**
Kernel main function
**/
void kmain()
{
	klog(LOG_INFO,"kmain","Booted into kernel proper.\n");
	
	//Start 
	device_manager_start();
	vfs_init();
	timing_init();
	init_vfs_devices();
	wd_init();
	klog(LOG_INFO,"kmain","Kernel took %dms to become fully operational!\n",timer_getHi());
	//Launch a shell
	vfs_node_t * shell = 0;
	for(int i = 0; i!= total_shells; i++)
	{
		shell = kopen(shells[i],0);
		if(!shell)
		{
			klog(LOG_WARN,"kmain","Can't launch %s: Executable not found\n",shells[i]);
		}
		else { break; }
	}
	if(!shell)
	{
		klog(LOG_PANIC,"kmain","No avalable shells to launch\n");
	}
	while(true)
	{
		wd_notify(WD_NOTIFY_KMAIN);
	}
}
