#include <log.h>
#include <error.h>
#include <beryllium/video.h>
#include <stdio.h>
#ifdef X86
#include <x86/low/isr.h>
#include <elf.h>
#endif
#define PANIC_MSG_BAR "=====================================PANIC======================================"

void panic(const char* reason)
{
	klog(LOG_PANIC,"panic",reason);
	halt();
}
void halt()
{
	klog(LOG_INFO,"halt","Halting!\n");
	#ifdef X86
	asm("cli");
	asm("hlt");
	#endif
	while(true)
	{
		
	}
}
#ifdef X86
void halt_regs(registers_t* regs)
{
	klog(LOG_INFO,"halt","Halting!\n");
	#ifdef X86
	asm("cli");
	printf("Registers:\n");
	printf("| eax 0x%X; ebx 0x%X; ecx 0x%X; edx 0x%X\n",regs->eax,regs->ebx,regs->ecx,regs->edx);
	printf("| ??? 0x%X; ebp 0x%X; err 0x%X; efl 0x%X\n",regs->useless_value,regs->ebp,regs->err_code,regs->eflags);
	printf("| usp 0x%X; eip 0x%X; esi 0x%X; edi 0x%X\n",regs->useresp,regs->ebp,regs->esi,regs->edi);
	printf("| cs 0x%X; ds 0x%X; es 0x%X; fs 0x%X\n",regs->cs,regs->ds,regs->es,regs->fs);
	printf("| gs  0x%X\n",regs->gs);
	halt();
	#endif
}
#endif
