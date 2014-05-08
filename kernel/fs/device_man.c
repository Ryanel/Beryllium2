#include <stdio.h>
#include <beryllium/vfs.h>
vfs_node_t *sysfs_device_create();
vfs_node_t *null_device_create();
vfs_node_t *zero_device_create();
vfs_node_t *serial_device_create();
//vfs_node_t *initrd_device_create(uint32_t location, uint32_t end);
void init_vfs_devices()
{
//	vfs_mount("/boot/", initrd_device_create(0, 0));
	vfs_mount("/dev/null", null_device_create());
	vfs_mount("/dev/zero", zero_device_create());
	//vfs_mount("/sys/", sysfs_device_create());
	vfs_mount("/dev/serial", serial_device_create());
	vfs_mount("/dev/",dev_device_create());
	//vfs_print_tree_node(vfs_tree->root, 0);
}
