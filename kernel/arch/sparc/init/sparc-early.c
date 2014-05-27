#include <sparc/bsd_openprom.h>
#include <types.h>
struct PromVec *PromVec;
struct BootDir *BootDir;

void sparc_early()
{	
 	PromVec->pv_printf("*** Hello World ***\n");
}

void sparc_start(struct PromVec *P, struct BootDir *BD)
{
	PromVec = P;
 	BootDir = BD;
 	sparc_early();
}

