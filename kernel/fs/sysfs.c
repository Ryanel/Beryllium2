#include <beryllium/vfs.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>

static vfs_node_t * sysfs_generic_create(char * name, read_type_t read_func) {
	vfs_node_t * fnode = malloc(sizeof(vfs_node_t));
	memset(fnode, 0x00, sizeof(vfs_node_t));
	fnode->inode = 0;
	strcpy(fnode->name, name);
	fnode->uid = 0;
	fnode->gid = 0;
	fnode->type    = VFS_FILE;
	fnode->read    = read_func;
	fnode->write   = NULL;
	fnode->open    = NULL;
	fnode->close   = NULL;
	fnode->readdir = NULL;
	fnode->finddir = NULL;
	return fnode;
}

static uint32_t null_func(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	return size;
}

struct sys_entry {
	int          id;
	char *       name;
	read_type_t  func;
};
static struct sys_entry sys_entries[] = {
	{1, "cpu",  null_func},
	{2, "mem",  null_func},
	{3, "uptime",   null_func},
	{4, "vfsinit",  null_func},
	{5, "version",  null_func},
	{6, "release", null_func},
};

struct dirent *readdir_sysfs (struct vfs_node *node, uint32_t index)
{
	if(index < 7)
	{
		struct dirent * out = malloc(sizeof(struct dirent));
		memset(out, 0x00, sizeof(struct dirent));
		out->ino = sys_entries[index].id;
		strcpy(out->name, sys_entries[index].name);
		return out;
	}
	
	return NULL;
}
vfs_node_t* finddir_sysfs(vfs_node_t *node, char *name)
{
	if (!name) return NULL;

	for (int i = 0; i < 7; ++i) {
		if (!strcmp(name, sys_entries[i].name)) {
			vfs_node_t * out = sysfs_generic_create(sys_entries[i].name, sys_entries[i].func);
			out->inode = node->inode;
			return out;
		}
	}

	return NULL;
}

vfs_node_t *sysfs_device_create()
{
	vfs_node_t * fnode = malloc(sizeof(vfs_node_t));
	memset(fnode, 0x00, sizeof(vfs_node_t));
	fnode->inode = 0;
	strcpy(fnode->name, "sysfs");
	fnode->uid = 0;
	fnode->gid = 0;
	fnode->permissions = VFS_PERMISSION_READ;
	fnode->flags   = VFS_DIRECTORY;
	fnode->read    = null_func;
	fnode->write   = NULL;
	fnode->open    = NULL;
	fnode->close   = NULL;
	fnode->readdir = readdir_sysfs;
	fnode->finddir = finddir_sysfs;
	return fnode;
}
