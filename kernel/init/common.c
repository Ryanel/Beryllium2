#include <system.h>
#include <stdio.h>
void serial_print_header(char *board,char *cpu_revision)
{
	for(int i = 0; i < 8; i++)
		serial_printf("----------");
	serial_printf("\n");
	serial_printf("Beryllium2 Kernel Version %s-%s:%s %s\n",SYSTEM_VERSION,SYSTEM_RELEASE,__DATE__,__TIME__);
	serial_printf("Output Device: Serial\n");
	serial_printf("Architecture: %s (%s)\n",ARCH_STRING,board);
	serial_printf("Processor: %s\n",cpu_revision);
	for(int i = 0; i < 8; i++)
		serial_printf("----------");
	serial_printf("\n");
}