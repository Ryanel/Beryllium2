#include <arm/intergrator-cp/drivers/serial.h>
#include <arm/intergrator-cp/arm4-xrq.h>
#include <beryllium/memory.h>
#include <stdio.h>
#include <log.h>
#include <types.h>
#include <boot/kmain.h>
#include <beryllium/video.h>
#include <beryllium/timer.h>
#include <system.h>
void icp_timer_setup();
void arm4_xrqsetup();

void arm_integrator_cp_early()
{
	qemu_pl110_start();
	gterminal_init();
	serial_print_header("Integrator-CP","ARM4");
	printf("Beryllium%c Kernel Version %s-%s built %s %s\n",253,SYSTEM_VERSION,SYSTEM_RELEASE,__DATE__,__TIME__);
	klog(LOG_INFO,"x86_early","Loading early enviroment platform for the %s\n", ARCH_STRING);
	klog(LOG_INFO,"arm_uart","Logging up on ARM (via. UART0)\n");
	uint32_t pc;
	asm("ldrh   %0, [pc, #6]" :"=r"(pc) : : "memory");
	klog(LOG_INFO,"arm_cpu","Program counter at ~0x%X\n",pc);
	arm4_xrqsetup();
	icp_timer_setup();
	uint32_t i = 0;
	klog(LOG_INFO,"arm_irq","Waiting for timer to fire... ");
	while(timer_getHi() == 0)
	{
		i++;
		if(i > 0xFFFFFF)
		{
			printf("it didn't.\n");
			klog(LOG_INFO,"arm_cpu","Halting!\n");
			return;
		}
	}
	printf("[OK]\n");
	memory_init();
	kmain();
	for(;;);
}