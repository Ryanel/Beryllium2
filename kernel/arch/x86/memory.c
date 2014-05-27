/**
Manages the memory

Hooks into src/memory.c
**/
#include <x86/paging.h>
#include <x86/page_allocator.h>
#include <x86/lib/placement_malloc.h>
#include <beryllium/memory.h>
#include <types.h>
#include <stdio.h>
#include <log.h>
#include <mutex.h>

///Page Directory
extern page_directory_t *kernel_directory;
///How many frames are free? Set to a default 0x10000000 to allow allocation BEFORE it's set. Because I am lazy.
uint32_t mem_pagefree = 0x1000000;
/**
The last page that was allocated. Used to speed up bitmap searches for free frames
Set by memory_init(); Running memory_mult_alloc_pages is possible without this set, but it takes a couple SECONDS on the first allocation,
as it must search every possible frame (about ~0x150000 frames). This is set to start the search at the last allocated frame (0x150000) so we
can search from there up.
**/
uint32_t mem_lastpage = 0;
///Kernel memory allocation mutex (boolean semaphore). This is NOT the lock on memory, only for the memory_alloc allocators.
mutex_t *mmac_lock;
/// The kernel's heap
//heap_t *kheap; 
/**
The kernel reserved area is exactly 4kb of memory. It is a sort of "shield" against the heap's conventional memory.
This is filled completely with 0xFF, and in the event of an EMERGENCY it can be written to to store up to 4kb of debug information.
This memory is not for conventional use; infact it is never used in the current implementation. It is just a shield.
**/
void* kernel_reserved_area;
/**
Allocates multple kernel heap pages and returns a pointer to the first page.
This function is not meant to be directly called, as this is not locked by the mutex mmac_lock.
Please use memory_alloc_pages() instead.
**/

void memory_set_total(uint32_t ram)
{
	//mem_pagefree = ram;
}

void* memory_mult_alloc_pages(int pages)
{
	int i, j;
	int block = -1;
	if(pa_frame_amount() < mem_pagefree)
	{
		klog(LOG_INFO,"mem_multi_alloc_pages","System ran out of free pages (%d)!\n",mem_lastpage);
		return NULL;
	}
	for(i = 0; (unsigned int)i < pa_frame_amount(); i++)
	{
		int pos = (i + mem_lastpage) % pa_frame_amount(); // Create a position based off of i.
		if ( (unsigned int)(pos + pages) > pa_frame_amount() )
		{
			i += pages - 2;
			continue;	
		}
		block = pos;
		
		for(j = 0;j < pages; j++)
		{
			if ( pa_test_frame(INDEX_FROM_BIT(pos + j)) ) 
			{
				block = -1;
				i = i + j;
				break;
			}
		}
		if ( block != -1 ) break;

	}
	if ( block == -1 ) return 0;
	for ( i = 0; i < pages; i++ )
		//pa_set_frame(INDEX_FROM_BIT(block + i));
		pa_alloc_frame(paging_get_page(block + i,1, kernel_directory), 0, 0);

	mem_lastpage = block + pages;

	return (void*)((unsigned int)block);
}
/**
Allocates multple pages and returns a pointer to the first page.

This function IS meant to be directly called, as this is locked by the mutex mmac_lock.
Please don't call memory_mult_alloc_pages() instead.
**/
void* memallocp(int pages)
{
	void *ptr = NULL;
	if(pages <= 0)
	{
		return 0;
	}

	mutex_lock(mmac_lock);
	ptr = memory_mult_alloc_pages(pages);
	if ( ptr == NULL )
	{
		klog(LOG_INFO,"memallocp","System ran out of free pages! Halting!\n",mem_lastpage); halt();
	}
		
	mutex_unlock(mmac_lock);
	return ptr;
}
/**
De-allocates pages
**/	
void memory_dealloc_pages(int pages)
{
	if(pages <= 0)
	{
		return;
	}

	mutex_lock(mmac_lock);
	int i = pages;
	int blk = mem_lastpage;
	while(i != 0)
	{
		pa_free_frame(paging_get_page(blk--,1, kernel_directory));
		i--;
	}
	mem_lastpage = pa_first_frame() * 0x1000;
	mutex_unlock(mmac_lock);
}
/**
Initializes Memory
Specificallly, it locks memory, initialises mem_lastpage, and allocates the kernel_reserved_area.
**/
void memory_init()
{
	mutex_init(mmac_lock);
	mem_lastpage = pa_first_frame() * 0x1000;
	memman_init();
}
/**
Allocates amount bytes, and returns a pointer to the beginning of the allocated amount.
**/
void *sbrk(size_t amount)
{
	if(amount == 0)
	{
		return (void*)((unsigned int)mem_lastpage);
	}
	uint32_t actual_amount = (amount / 0x1000);
	if(actual_amount == 0)
	{
		actual_amount++;
	}
	//printf("sbrk: allocating %d pages to cover 0x%X bytes\n",actual_amount,amount);
	void *tmp = memallocp(actual_amount);
	return tmp;
}
/**
Deallocates memory(pages) from end_data_segment forward.
\todo Implement brk
**/
int brk(void *end_data_segment)
{
	return 0;
}
void memory_parse_grub()
{

}
