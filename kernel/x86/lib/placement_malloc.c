#include <x86/lib/placement_malloc.h>
#include <types.h>

extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

uint32_t placement_kmalloc(uint32_t sz)
{
	uint32_t tmp = placement_address;
	placement_address += sz;
	return tmp;
}
uint32_t placement_kmalloc_a(uint32_t sz, int align)
{
	if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
	{
		// Align it.
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	uint32_t tmp = placement_address;
	placement_address += sz;
	return tmp;
}
uint32_t placement_kmalloc_ap(uint32_t sz, int align, uint32_t *phys)
{
	if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
	{
		// Align it.
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys)
	{
		*phys = placement_address;
	}
	uint32_t tmp = placement_address;
	placement_address += sz;

	return tmp;
}
