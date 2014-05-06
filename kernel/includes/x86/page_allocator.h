#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H
#include <types.h>
#include <x86/paging.h>
///Page index from address
#define INDEX_FROM_BIT(b) (b / 0x20)
///Page offest from address
#define OFFSET_FROM_BIT(b) (b % 0x20)
///Init the allocator
void init_page_allocator();
///Set the frame that governs address to used
void pa_set_frame(uint32_t address);
///Set the frame that governs address to free
void pa_clear_frame(uint32_t address);
///Is the frame that governs address used?
uint32_t pa_test_frame(uint32_t address);
///First free frame
uint32_t pa_first_frame();
///How many possible frames?
uint32_t pa_frame_amount();
///Allocate a frame from page
void pa_alloc_frame(page_t *page, int kernel, int rw);
///Free a frame
void pa_free_frame(page_t *page);
#endif
