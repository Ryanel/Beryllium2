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
#include <beryllium/thread.h>
#include <error.h>
#include <string.h>

#ifdef X86
void x86_switch_to_usermode();
#endif

void init_vfs_devices();
void kshell_init();
void kshell_parse_char(char);
char kb_read();
void device_manager_start();

void hello()
{
	klog(LOG_INFO,"hello_world","Hello, world!\n");
}

/**
Kernel main function
**/
void kmain()
{
	klog(LOG_INFO,"kmain","Booted into kernel proper.\n");
	
	//Start
	klog(LOG_INFO,"kmain","Starting VFS\n");
	vfs_init(); 
	device_manager_start();
	timing_init();
	init_vfs_devices();
	wd_init();
	klog(LOG_INFO,"kmain","Kernel took %dms to become fully operational!\n",timer_getHi());
	
	initrd_init();
	
	thread_t * thread = threading_start();
	thread_t * test = thread_create(0,0, hello);
	
	thread_switch(test);
	
	klog(LOG_PANIC,"init","No init found. Dropping login\n");
	while(true)
	{
		wd_notify(WD_NOTIFY_KMAIN);
	}
}
