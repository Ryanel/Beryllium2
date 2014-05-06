/**
Page Allocator - Simple page allocator with bitsets.
**/

#include <types.h>
#include <string.h>
#include <x86/lib/placement_malloc.h>
#include <x86/page_allocator.h>
#include <x86/paging.h>
#include <log.h>
//Undefine Debug so there is no output
#undef DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif
uint32_t *frame; //Pointer to first frame, first index. The actual bitmap
uint32_t frame_amount; //How many frames CAN there be?
uint32_t mem_end = 0xF000000; //Where does memory end. Default's to all addressable ram
uint32_t mem_end_aligned; //Where does memory end, page aligned.

uint32_t pa_frame_amount()
{
	return frame_amount;
}

void pa_set_frame(uint32_t address)
{
	uint32_t frame_addr = address / 0x1000;
	uint32_t index = INDEX_FROM_BIT(frame_addr);
	uint32_t offset = OFFSET_FROM_BIT(frame_addr);
	frame[index] |= (0x1 << offset);
}
void pa_clear_frame(uint32_t address)
{
	uint32_t frame_addr = address / 0x1000;
	uint32_t index = INDEX_FROM_BIT(frame_addr);
	uint32_t offset = OFFSET_FROM_BIT(frame_addr);
	frame[index] &= ~(0x1 << offset);
}
uint32_t pa_test_frame(uint32_t address)
{
	uint32_t frame_addr = address / 0x1000;
	uint32_t index = INDEX_FROM_BIT(frame_addr);
	uint32_t offset = OFFSET_FROM_BIT(frame_addr);
	return (frame[index] & (0x1 << offset));
}

uint32_t pa_first_frame()
{
	uint32_t i,j;
	for (i = 0; i < INDEX_FROM_BIT(frame_amount); i++)
	{
		if(frame[i] != 0xFFFFFFFF)
		{
			for(j = 0; j < 32; j++)
			{
				uint32_t testFrame = 0x1 << j;
				if (!(frame[i] & testFrame)) {
					return i * 0x20 + j;
				}
			}
		}
	}
	return -1;
}

void pa_alloc_frame(page_t *page, int kernel, int rw)
{
	if(kernel > 1)
	{
		klog(LOG_SEVERE,"PAGEA","Attempted to allocate page 0x%X with privilage level (%d;K=%d)!\n",&page,kernel,rw);
		return;
	}
	if(page->frame != 0)
	{
		return;
	}

	uint32_t index = pa_first_frame();
	if(index == (uint32_t)-1) //OxFFFFFFFF
	{
		klog(LOG_SEVERE,"PANIC","Page Allocator: no free frames!\n");
		klog(LOG_SEVERE,"PANIC","idex: 0x%X. passed frame: 0x%X.\n",index,page->frame);
		asm("cli");
		asm("hlt");
		return;
	}
	pa_set_frame(index*0x1000);
	page->present = 1;
	page->rw = (rw)?1:0;
	page->user = (kernel)?0:1;
	page->frame = index;
}

void pa_free_frame(page_t *page)
{
	uint32_t ftf; //Frame to free
	if(!(ftf=page->frame))
	{
		return;
	}
	else
	{
		pa_clear_frame(ftf); //Free it!
		page->frame = 0;
	}
}
void init_page_allocator()
{
	//printf("Allocating pages...\n");
	mem_end_aligned = (mem_end & 0xFFFFF000);
	#ifdef DEBUG
	printf("Page Allocator:\n");
	int amm_alloc_mb = ((mem_end_aligned/1024)/1024);
	printf("---> Can allocate for 0x%X (~%d MB) of ram\n",mem_end_aligned,amm_alloc_mb);
	#endif

	frame_amount = mem_end_aligned / 4;
	frame = (uint32_t*)placement_kmalloc(INDEX_FROM_BIT(frame_amount));
	memset(frame, 0, INDEX_FROM_BIT(frame_amount)); //Clear frame

	#ifdef DEBUG
	printf("---> Allocatable frames: 0x%X (%d), bitmap starting at 0x%X\n",frame_amount,frame_amount,frame);
	#endif
}

