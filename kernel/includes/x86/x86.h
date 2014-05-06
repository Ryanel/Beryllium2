#ifndef X86_H
#define X86_H
///Initialises x86 hardware
void init_x86();
///Sets up the GDT
void gdt_setup();
///Sets up the IDT
int idt_setup();
///Sets up isrs's
int isrs_setup();
///Sets up interrupt requests
void irq_install();
#endif
