#Memory

* Beryllium requires a minimum of 2MB of ram to run (confortably)
* Virtual addressing is supported (with memmap(3))

## Memory map
x86

```
0x00000000 - 0x00100000 Used
0x01000000 - 0x00200000 Grub + Kernel + Modules (phys)
KERNEL END - 0xE0000000 Memory avalable for programs
0xE0000000 - 0xFFFFFFFF Kernel modules and datastructures (virt)
```
ARM

```
TODO
```
## Memory Manager

### Tier 0 - Hardware

Paging is defined here, therefore it does not be to described by this document

### Tier 1 - Page Allocater

Defined in src/arch/page_allocator.c

The page allocator simply marks what pages are free or used in a bitmap.

### Tier 2 - Memory Manager

Defined in src/arch/memory.c

This memory manager uses _regions_ to mark which memory is in use