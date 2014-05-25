#include <types.h>
/**
Manages the memory

Hooks into src/memory.c
**/
#include <beryllium/memory.h>
#include <types.h>
#include <stdio.h>
#include <log.h>
#include <mutex.h>

uint64_t mem_allocate_starting = 0x10000;
///Kernel memory allocation mutex (boolean semaphore). This is NOT the lock on memory, only for the memory_alloc allocators.
mutex_t *mmac_lock;

/**
The kernel reserved area is exactly 4kb of memory. It is a sort of "shield" against the heap's conventional memory.
This is filled completely with 0xFF, and in the event of an EMERGENCY it can be written to to store up to 4kb of debug information.
This memory is not for conventional use; infact it is never used in the current implementation. It is just a shield.
**/
void* kernel_reserved_area;
/**
Allocates multple pages and returns a pointer to the first page.
This function is not meant to be directly called, as this is not locked by the mutex mmac_lock.
Please use memory_alloc_pages() instead.
**/
void* memory_mult_alloc_pages(int pages)
{
	int to = mem_allocate_starting;
	mem_allocate_starting += (pages * 0x1000);
	return (void*)((unsigned int)to);
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
		printf("memory_alloc_pages(%d):Out of memory! Returning NULL\n");
	mutex_unlock(mmac_lock);
	return ptr;
}
/**
De-allocates pages
**/	
void memory_dealloc_pages(int pages)
{
	///TODO: Make this work
}
/**
Initializes Memory

Specificallly, it locks memory, initialises mem_lastpage, and allocates the kernel_reserved_area.
**/
void memory_init()
{
	mutex_init(mmac_lock);
	mutex_lock(mmac_lock);
	klog(LOG_INFO,"memory_init","Initialising and populating memory...\n");
	klog(LOG_DEBUG,"memory_init","Marked 0x%X (%d) bytes as dirty\n",mem_allocate_starting,mem_allocate_starting);
	mutex_unlock(mmac_lock);
	klog(LOG_DEBUG,"memory_init","Allocating Kernel Reserved Area...\n");
	kernel_reserved_area = memallocp(1);
	klog(LOG_DEBUG,"memory_init","Created heap...!\n");
	klog(LOG_INFO,"memory_init","Done allocating initial memory!\n");
}
/**
Allocates amount bytes, and returns a pointer to the beginning of the allocated amount.
**/
void *sbrk(size_t amount)
{
	if(amount == 0)
	{
		return (void*)((unsigned int)mem_allocate_starting);
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