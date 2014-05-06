#include <beryllium/vfs.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>
uint32_t read_dev(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_dev(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_dev(vfs_node_t *node, unsigned int flags);
void close_dev(vfs_node_t *node);

uint32_t read_dev(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	return 0;
}

uint32_t write_dev(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	return 0;
}

void open_dev(vfs_node_t * node, unsigned int flags) {
	return;
}

void close_dev(vfs_node_t * node) {
	return;
}

vfs_node_t *dev_device_create()
{
	vfs_node_t * fnode = malloc(sizeof(vfs_node_t));
	memset(fnode, 0x00, sizeof(vfs_node_t));
	fnode->inode = 0;
	strcpy(fnode->name, "devfs");
	fnode->uid = 0;
	fnode->gid = 0;
	fnode->permissions = VFS_PERMISSION_READ;
	fnode->flags   = VFS_DIRECTORY;
	fnode->read    = read_dev;
	fnode->write   = write_dev;
	fnode->open    = open_dev;
	fnode->close   = close_dev;
	fnode->readdir = NULL;
	fnode->finddir = NULL;
	return fnode;
}
