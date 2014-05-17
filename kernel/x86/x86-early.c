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
void x86_early(int magic,multiboot_info_t *mboot)
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
	
	printf("Beryllium%c Kernel Version %s-%s:%s %s\n",253,SYSTEM_VERSION,SYSTEM_RELEASE,__DATE__,__TIME__);
	
	klog(LOG_INFO,"x86_early","Loading early enviroment platform for the x86 processor\n");
	
	if(magic!=0x2BADB002)
	{
		klog(LOG_PANIC,"multiboot","Multiboot magic verfication failed\n",magic);
		klog(LOG_PANIC,"multiboot","+------------------------+\n");
		klog(LOG_PANIC,"multiboot","|Reciept for: Multiboot  |\n");
		klog(LOG_PANIC,"multiboot","|Magic: 0x%-015X|\n",magic);
		klog(LOG_PANIC,"multiboot","|Bootloader:%-13s|\n",mboot->boot_loader_name);
		klog(LOG_PANIC,"multiboot","+------------------------+\n");
		return;
	}
	else
	{
		klog(LOG_INFO,"multiboot","Verification passed\n");
		klog(LOG_INFO,"multiboot","Booted by %s\n",mboot->boot_loader_name);
		klog(LOG_INFO,"multiboot","System has %d MB of ram avalable\n",mboot->mem_upper / 1024);
	}
	//Multiboot verified, we can do things
	
	memory_set_total(mboot->mem_upper * 0x1000);
	if(mboot->mods_count)
	{
		multiboot_mod_t * mod = (multiboot_mod_t*) mboot->mods_addr;
		uint32_t module_start = mod->mod_start;
		uint32_t module_end = mod->mod_end;
		initrd_set_location(module_start, module_end);
		placement_address = module_end;
	}
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
