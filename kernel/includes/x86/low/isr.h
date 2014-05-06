#ifndef X86_ISR_H
#define X86_ISR_H
///A representation of pushed registers for interrupts
struct regs
{
	unsigned int gs, fs, es, ds;      /* pushed the segs last */
	unsigned int edi, esi, ebp, useless_value, ebx, edx, ecx, eax;  /* pushed by pusha. useless value is esp */
	unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};
typedef struct regs registers_t;
#endif
