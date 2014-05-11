#include <types.h>
#include <x86/paging.h>
#include <log.h>
#include <x86/page_allocator.h>
#include <x86/lib/placement_malloc.h>
#include <string.h>
#include <x86/low/irq.h>
#include <beryllium/memory.h>
#undef DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

extern uint32_t end; //End of kernel
extern uint32_t placement_address;
uint32_t eok_aligned; //End of kernel page aligned.

page_directory_t *kernel_directory;
page_directory_t *current_directory;

page_t * paging_get_page(uint32_t address,int make, page_directory_t * dir)
{
	address /= 0x1000;
	uint32_t table_index = address / 1024;
	if (dir->tables[table_index])
	{
		return &dir->tables[table_index]->pages[address % 1024];
	} 
	else if(make)
	{
		uint32_t temp;
		dir->tables[table_index] = (page_table_t *)placement_kmalloc_ap(sizeof(page_table_t),1, (uint32_t *)(&temp));
		memset(dir->tables[table_index], 0, sizeof(page_table_t));
		dir->tablesPhysical[table_index] = temp | 0x7; /* Present, R/w, User */
		return &dir->tables[table_index]->pages[address % 1024];
	}
	else {
		return 0;
	}
}

void paging_switch_directory(page_directory_t * dir)
{
	current_directory = dir;
	asm volatile ("mov %0, %%cr3":: "r"(dir->physicalAddr));
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0":: "r"(cr0));
}
void paging_fault(struct regs *r)
{
	uint32_t faulting_address;
   	asm ("mov %%cr2, %0" : "=r" (faulting_address));
	klog(LOG_SEVERE,"PF","Page fault!\n");
	int present   = !(r->err_code & 0x1); // Page not present
	int rw = r->err_code & 0x2;           // Write operation?
	int us = r->err_code & 0x4;           // Processor was in user-mode?
	int reserved = r->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
	int id = r->err_code & 0x10;          // Caused by an instruction fetch?
	klog(LOG_SEVERE,"PF","Addr:0x%X. present %d rw %d us %d res %d id %d\n",faulting_address,present,rw,us,reserved,id);
}
void paging_init()
{
	init_page_allocator();
	uint32_t phys;
	kernel_directory = (page_directory_t *)placement_kmalloc_ap(sizeof(page_directory_t),1,&phys);
	memset(kernel_directory, 0, sizeof(page_directory_t));
	uint32_t i = 0;
	for (i=0; i < placement_address + 0x1000; i += 0x1000) {
		pa_alloc_frame(paging_get_page(i, 1, kernel_directory), 0, 0);
	}
	#ifdef DEBUG
	printf("Allocated 0x%X frames...\n",i);
	#endif
	kernel_directory->physicalAddr = (uint32_t)kernel_directory->tablesPhysical;
	register_interrupt_handler(14,&paging_fault);
	paging_switch_directory(kernel_directory);
}
