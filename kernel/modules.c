#include <lib/list.h>
#include <log.h>
int boot_modules = 0;

typedef struct {
	int test;
} module_raw_t;

list_t * modules;

void module_add_raw(module_raw_t * module)
{
	list_insert(modules, module);
}

void modloader_init()
{
	klog(LOG_INFO,"modloader_init","Initialising...\n");
	modules = list_create();
}