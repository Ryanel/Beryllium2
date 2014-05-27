#include <arm/intergrator-cp/arm4-xrq.h>
#include <beryllium/timer.h>

void timer_switchDevice(int);

void icp_timer_setup()
{
	uint32_t		*t0mmio;
	uint32_t		*picmmio;
	picmmio = (uint32_t*)0x14000000;
	picmmio[PIC_IRQ_ENABLESET] = (1<<5) | (1<<6) | (1<<7);

	t0mmio = (uint32_t*)0x13000000;
	t0mmio[REG_LOAD] = 0x1000;
	t0mmio[REG_BGLOAD] = 40000;			
	t0mmio[REG_CTRL] = CTRL_ENABLE | CTRL_MODE_PERIODIC | CTRL_SIZE_32 | CTRL_DIV_NONE | CTRL_INT_ENABLE;
	t0mmio[REG_INTCLR] = ~0;
	timer_switchDevice(0x2);
	klog(LOG_INFO,"icp_timer_setup","Timer => %s\n",timer_getDeviceString(timer_getDevice()));
}
