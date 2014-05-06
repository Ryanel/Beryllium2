#include <beryllium/vfs.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>
#ifdef X86
#include <x86/drivers/serial.h>
#endif
uint32_t read_serial(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_serial(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_serial(vfs_node_t *node, unsigned int flags);
void close_serial(vfs_node_t *node);

uint32_t read_serial(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	return 0;
}

uint32_t write_serial(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	for(int i = 0; i != size; i++)
	{
		serial_write(buffer[i]);
	}
}

void open_serial(vfs_node_t * node, unsigned int flags) {
	return;
}

void close_serial(vfs_node_t * node) {
	return;
}

vfs_node_t *serial_device_create()
{
	vfs_node_t * fnode = malloc(sizeof(vfs_node_t));
	memset(fnode, 0x00, sizeof(vfs_node_t));
	fnode->inode = 0;
	strcpy(fnode->name, "serial");
	fnode->uid = 0;
	fnode->gid = 0;
	fnode->permissions = VFS_PERMISSION_READ | VFS_PERMISSION_WRITE;
	fnode->flags   = VFS_FILE | VFS_CHARDEVICE;
	fnode->read    = read_serial;
	fnode->write   = write_serial;
	fnode->open    = open_serial;
	fnode->close   = close_serial;
	fnode->readdir = NULL;
	fnode->finddir = NULL;
	return fnode;
}

