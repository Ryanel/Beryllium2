/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)
SECTIONS
{
  . = 1M;
  .__mbHeader : {
      *(.__mbHeader)
  }
  .text :
  {
    code = .;
    *(.text)
    . = ALIGN(4096);
  }

  .data :
  {
     data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }

  end = .; _end = .;
}