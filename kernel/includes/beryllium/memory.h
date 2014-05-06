#ifndef X86_MEMORY_H
#define X86_MEMORY_H
#include <types.h>
#include <lib/ordered_array.h>
//void* memory_mult_alloc_pages(int pages);
void* memory_alloc_pages(int pages);
void memory_init();
void memory_set_total(uint32_t ram);
void *sbrk(size_t amount);
#endif
