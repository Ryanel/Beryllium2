#include <boot/kmain.h>
#include <boot/multiboot.h>
#include <system.h>

#include <terminal.h>
#include <stdio.h>
#include <log.h>

#include <x86/x86.h>
#include <beryllium/memory.h>
#include <x86/drivers/serial.h>
#include <x86/cpuid.h>
#include <elf.h>

#ifdef ENABLE_SERIAL
void serial_print_header();
#endif

void pit_init();
void kb_init();
void paging_init();
void memory_init();

extern uint32_t placement_address;

/**
 Get the kernel up and running as fast as possible
**/
void x86_early(int magic,struct multiboot *mboot)
{
	#ifdef IS_RELEASE
	klog_setmask(LOG_INFO);
	#else
	klog_setmask(LOG_DEBUG);
	#endif
	terminal_init();

	#ifdef ENABLE_SERIAL
	serial_init();
	serial_print_header("Intel Compatible","i586");
	#endif
	get_x86_cpu_info();
	printf("Kernel Version:\n");
	printf("Beryllium%c Kernel Version %s-%s:%s %s\n",253,SYSTEM_VERSION,SYSTEM_RELEASE,__DATE__,__TIME__);
	
	
	klog(LOG_INFO,"x86_early","Loading early enviroment platform for the x86 processor\n");
	
	if(magic!=0x2BADB002)
	{
		klog(LOG_PANIC,"x86_early","Multiboot Magic number: 0x%X! It should equal 0x2BADB002. Halting...\n",magic);
		return;
	}
	//Set kernel properties from multiboot info.
	//klog(LOG_INFO,"MBT","%d modules loaded with kernel\n",mboot->mods_count);
	boottime_module_set(mboot->mods_count);
	klog(LOG_INFO,"x86_early","System has %dMB of ram avalable\n",mboot->mem_upper / 1024);
	memory_set_total(mboot->mem_upper * 0x1000);
	asm("cli");

	gdt_setup();
	idt_setup();
	isrs_setup();
	irq_install();
	kb_init();
	paging_init();
	asm("sti");
	memory_init();
	//Populate Modules List
	modloader_init();
	pit_init();
	kmain();
}
