#include <stdio.h>
#include <types.h>
#include <x86/cpuid.h>

uint32_t cpu_features;
uint32_t cpu_features_extended;

void get_x86_cpu_info()
{
	uint32_t eax,ebx,ecx,edx = 0;
	cpuid(0x1,eax,ebx,ecx,edx);
	cpu_features = edx;
	cpu_features_extended = ecx;
	
}

int cpu_feature_avalable(uint32_t feature)
{
	if(cpu_features & feature)
	{
		return 1;
	}
	return 0;
}

int cpu_feature_extended_avalable(uint32_t feature)
{
	if(cpu_features_extended & feature)
	{
		return 1;
	}
	return 0;
}