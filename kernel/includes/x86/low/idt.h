#ifndef X86_IDT_H
#define X86_IDT_H
///An entry in the idt
struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));
///Pointer to the idt
struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));
extern void idt_load();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
#endif
