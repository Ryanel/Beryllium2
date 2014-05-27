#include <types.h>
#include <arm/intergrator-cp/arm4-xrq.h>
#include <log.h>
#include <error.h>
#include <beryllium/timer.h>
#define KEXP_TOPSWI \
	uint32_t			lr; \
	asm("mov sp, %[ps]" : : [ps]"i" (KSTACKEXC)); \
	asm("push {lr}"); \
	asm("push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}"); \
	asm("mov %[ps], lr" : [ps]"=r" (lr));
 
#define KEXP_TOP3 \
	uint32_t			lr; \
	asm("mov sp, %[ps]" : : [ps]"i" (KSTACKEXC)); \
	asm("sub lr, lr, #4"); \
	asm("push {lr}"); \
	asm("push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}"); \
	asm("mov %[ps], lr" : [ps]"=r" (lr));
 
#define KEXP_BOT3 \
	asm("pop {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}"); \
	asm("LDM sp!, {pc}^");

void arm4_xrq_handler(uint32_t lr, uint32_t type);
void __attribute__((naked)) k_exphandler_irq_entry() { KEXP_TOP3;   arm4_xrq_handler(lr, ARM4_XRQ_IRQ); KEXP_BOT3; }
void __attribute__((naked)) k_exphandler_fiq_entry() { KEXP_TOP3;   arm4_xrq_handler(lr, ARM4_XRQ_FIQ); KEXP_BOT3; }
void __attribute__((naked)) k_exphandler_reset_entry() { KEXP_TOP3; arm4_xrq_handler(lr, ARM4_XRQ_RESET); KEXP_BOT3; }
void __attribute__((naked)) k_exphandler_undef_entry() { KEXP_TOP3; arm4_xrq_handler(lr, ARM4_XRQ_UNDEF); KEXP_BOT3; }	
void __attribute__((naked)) k_exphandler_abrtp_entry() { KEXP_TOP3; arm4_xrq_handler(lr, ARM4_XRQ_ABRTP); KEXP_BOT3; }
void __attribute__((naked)) k_exphandler_abrtd_entry() { KEXP_TOP3; arm4_xrq_handler(lr, ARM4_XRQ_ABRTD); KEXP_BOT3; }
void __attribute__((naked)) k_exphandler_swi_entry() { KEXP_TOPSWI; arm4_xrq_handler(lr, ARM4_XRQ_SWINT); KEXP_BOT3; }

void arm4_xrqinstall(uint32_t ndx, void *addr)
{
	uint32_t *v;
	v = (uint32_t*)0x0;
	v[ndx] = 0xEA000000 | (((uintptr_t)addr - 8 - (4 * ndx)) >> 2);
}

uint32_t arm4_cpsrget()
{
    uint32_t r;
    asm volatile("mrs %[ps], cpsr" : [ps]"=r" (r));
    return r;
}

uint32_t arm4_spsrget()
{
    uint32_t r;
 
    asm("mrs %[ps], spsr" : [ps]"=r" (r));
    return r;
}

void arm4_cpsrset(uint32_t r)
{
    asm volatile("msr cpsr, %[ps]" : : [ps]"r" (r));
}

void arm4_xrqenable_fiq()
{
    arm4_cpsrset(arm4_cpsrget() & ~(1 << 6));
}

void arm4_xrqenable_irq()
{
    arm4_cpsrset(arm4_cpsrget() & ~(1 << 7));
}

void arm4_xrqsetup()
{
	klog(LOG_INFO,"arm4_xrqsetup","Setting up interrupts...\n");
	arm4_xrqinstall(ARM4_XRQ_RESET, &k_exphandler_reset_entry);
	arm4_xrqinstall(ARM4_XRQ_UNDEF, &k_exphandler_undef_entry);
	arm4_xrqinstall(ARM4_XRQ_SWINT, &k_exphandler_swi_entry);
	arm4_xrqinstall(ARM4_XRQ_ABRTP, &k_exphandler_abrtp_entry);
	arm4_xrqinstall(ARM4_XRQ_ABRTD, &k_exphandler_abrtd_entry);
	arm4_xrqinstall(ARM4_XRQ_IRQ, &k_exphandler_irq_entry);
	arm4_xrqinstall(ARM4_XRQ_FIQ, &k_exphandler_fiq_entry);
	arm4_cpsrset(arm4_cpsrget() & ~(1 << 7));
	asm("swi 4");
}


void arm4_xrq_handler(uint32_t lr, uint32_t type)
{
	uint32_t *picmmio;
	
	// Is this a IRQ?
	if (type == ARM4_XRQ_IRQ)
	{
		picmmio = (uint32_t*)0x14000000;
		if (picmmio[PIC_IRQ_STATUS] & 0x20)
		{
			uint32_t *t0mmio;
			t0mmio = (uint32_t*)0x13000000;
			t0mmio[REG_INTCLR] = 1;			/* according to the docs u can write any value */
			timer_recieveTick(0x2);
		}
		else
		{
			
		}
		return;
	}
	// Is this a software interrupt?
	if (type == ARM4_XRQ_SWINT)
	{
		uint32_t swi;
		//Yes
		swi = ((uint32_t*)((uintptr_t)lr - 4))[0] & 0xffff;
		if (swi == 4)
		{
			klog(LOG_INFO,"arm_xrq","SWI's are working! (swi 0x%x,cpsr 0x%X spsr 0x%X)\n",swi,arm4_cpsrget(), arm4_spsrget());
		}
		return;
	}
	if (type != ARM4_XRQ_IRQ && type != ARM4_XRQ_FIQ && type != ARM4_XRQ_SWINT)
	{
		/*
			Ensure, the exception return code is correctly handling LR with the
			correct offset. I am using the same return for everything except SWI, 
			which requires that LR not be offset before return.
		*/
		klog(LOG_PANIC,"arm_xrq","Recieved unhandled interrupt (type %d)!\n",type);
		panic("Unhandled interrupt\n");
	}
	//Okay were assuming this is a timer interrupt.
}