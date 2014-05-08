#include <lib/list.h>
#include <log.h>
int boot_modules = 0;

typedef struct {
	int add;
	int y;
} module_raw_t;

list_t * modules_preload;

int boottime_module_added()
{
	return boot_modules;
}

void boottime_module_set(int mods)
{
	boot_modules = mods;
}

void boottime_module_add()
{
}
void modloader_init()
{
	klog(LOG_INFO,"modloader_init","Initialising...\n");
	modules_preload = list_create();
}