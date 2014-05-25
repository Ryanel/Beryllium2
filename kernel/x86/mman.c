/**
 * Beryllium Regioned Memory Manager - Sample
 * Written by Corwin McKnight
 * WARNING: This file deals with memory but CANNOT use allocation. Make sure things are dynamic ONLY if we got the heap up
**/
#include <stdio.h>
#include <types.h>
#include <string.h>
#include <stdlib.h>
#include <beryllium/memory.h>
#include <log.h>
#define PAGE_SIZE 0x1000
#define DEBUG 1
typedef struct mman_region {
	struct 		mman_region  * prev;
	struct 		mman_region * next;
	uint8_t		used;				// Is this used?
	uint8_t 	type;				// 0 - Kernel, 1 - Heap, 2 - Usercode, 3 - MMIO, 4 - Reserved, 6 - System
	uint8_t 	flags;				// VK-----I (Virtual, Kernel, RESV * 5, Initialised)
	uintptr_t 	start;				// Must be page aligned
	size_t 		size;
} mman_region_t;

/// 0xFF preallocated memory regions until we can get a bootstrap memory system online.
mman_region_t mman_prealloc[0xFF];
mman_region_t * mman_region;
uint8_t mman_prealloc_ptr = 0;

/// The status of the memory manager (to deny memory allocations and etc)
uint8_t mman_status = 0;

/**
 * Signal the memory manager about a new region.
**/
int memsig(uintptr_t start, size_t size, uint8_t type, uint8_t flags)
{
	klog(LOG_DEBUG,"memsig","Creating memory region 0x%X-0x%X...\n",start,start+size);
	if(size == 0)
	{
		printf("memsig: size is 0!\n");
		return 1;
	}
	if (mman_status == 0) //We can't do anything if memory_init hasn't been called!
	{
		return 2;
	}
	mman_region_t * region;
	if (mman_status == 1) //memory_init called, but no heap defined!
	{
		region = &mman_prealloc[mman_prealloc_ptr++];
	}
	else
	{
		region = malloc(sizeof(mman_region_t));
	}
	region->start = start;
	region->size  = size;
	region->type  = type;
	region->flags = flags;
	region->used  = 0x1;
	if(region->type == 0x1 && mman_status == 0x1)
	{
		mman_status = 0x2; //We got a heap!
	}
	// Its detached from the main list, so we need to find the next free item
	mman_region_t * parent_ptr = mman_region;
	while (1)
	{
		if(parent_ptr == NULL) // On first memsig
		{
			parent_ptr = region;
			region->next = NULL;
			break;
		}
		else if(parent_ptr->next == NULL)
		{
			parent_ptr->next = region;
			region->prev     = parent_ptr;
			region->next     = NULL;
			break;
		}
		else
		{
			parent_ptr = parent_ptr->next;
		}
	}
	//printf("new region: 0x%08X - 0x%08X\n",region->start,region->size + region->start);
	return 0;
}

/**
 * Safely modify the memory region(to prevent boundery fails)
**/
int memresize(mman_region_t * region, size_t size)
{
	klog(LOG_DEBUG,"memresize","Resizing 0x%X - 0x%X to 0x%X bytes\n",region->start,region->start + region->size,size);
	unsigned int addr_size_new = size + region->start;
	mman_region_t * parent_ptr = mman_region;
	while (1)
	{
		if(parent_ptr == NULL) // If this n
		{
			return 2;
		}
		else if(parent_ptr == region)
		{

		}
		else
		{
			if(parent_ptr->start > region->start)
			{
				if(parent_ptr->start < addr_size_new)
				{
					klog(LOG_DEBUG,"memresize","Resizing 0x%X - 0x%X failed: EMEMBOUNDRY\n",region->start,region->start + region->size);
					return 1;
				}
			}
		}
		if(parent_ptr->next == NULL)
		{
			//We didn't find an offending region
			region->size = size; //Commit changes
			break;
		}
		parent_ptr = parent_ptr->next;
	}
	return 0;
}

mman_region_t * get_kernel_heap()
{
	mman_region_t * parent_ptr = mman_region;
	while (1)
	{
		if(parent_ptr == NULL) // If this n
		{
			return NULL;
		}
		else
		{
			if(parent_ptr->type == 0x1)
			{
				return parent_ptr;
			}
		}
		parent_ptr = parent_ptr->next;
	}
}

extern uint32_t _end;

void *mem_heap_sbrk(size_t amount) //This is the kernel's sbrk
{
	mman_region_t * heap = get_kernel_heap();
	if(amount == 0)
	{
		return (void*)(heap->start + heap->size);
	}
	uint32_t actual_amount = (amount / 0x1000);
	if(actual_amount == 0)
	{
		actual_amount++;
	}
	void *tmp =  memallocp(actual_amount);
	return tmp;
}

/**
 * Initialize the memory manager. Refrain from using memsig & malloc
**/
void memman_init()
{
	//printf("Starting memory manager...\n");
	memset(mman_prealloc,0,sizeof(mman_region_t) * 0xFF);
	mman_region = &mman_prealloc[0]; //Assign it to the first preallocated block
	mman_status = 1;
	memsig(0x0, (uintptr_t)&_end, 0x0, 0x21); // Kernel - to protect it
	memsig((((uintptr_t)&_end / 0x1000) * 0x1000) + 0x1000,0x100000, 0x1, 0x21); // Kernel heap
}
