#include <types.h>
#include <log.h>
#include <stdio.h>
#include <beryllium/fs/initfs.h>
#include <beryllium/vfs.h>
#include <lib/tree.h>
#include <string.h>
#include <stdlib.h>
uint32_t initrd_exists = 0;
uint32_t initrd_startaddr = 0;
uint32_t initrd_endaddr = 0;

superheader_t * initfs_header;
fileheader_t  * initfs_fileheaders;

vfs_node_t  * initrd_mountpoint;
vfs_node_t  * initrd_nodes;
void initrd_set_location(uint32_t start, uint32_t end)
{
	initrd_startaddr	= start;
	initrd_endaddr		= end;
	initrd_exists		= 1;
}


vfs_dirent_t * initrd_readdir(vfs_node_t *node, uint32_t index)
{
	printf("readdir\n");
	vfs_dirent_t * dirent = malloc(sizeof(vfs_dirent_t));
	if (index >= initfs_header->headers)
		return 0;
	printf("found\n");
	strcpy(dirent->name, initrd_nodes[index].name);
	dirent->name[strlen(initrd_nodes[index].name)] = 0; // Make sure the string is NULL-terminated.
	dirent->ino = initrd_nodes[index].inode;
	return dirent;
}

vfs_node_t * initrd_finddir(vfs_node_t *node,const char *name)
{
	for(unsigned int i = 0; i < initfs_header->headers; i++)
	{
		printf("initrd: searching for %s\n",name);
		if(!strcmp(name, (const char *)initrd_nodes[i].name))
		{
			return &initrd_nodes[i];
		}
	}
	return NULL;
}

int initrd_verify(uint32_t * addr) //TODO: Do actual error checking
{
	if(addr[0]=='I')
	{
		return 1;
	}
	return 0;
}

void initrd_init() //FIXME: All return same name
{
	if(!initrd_exists)
	{
		klog(LOG_PANIC,"initrd_init","No initrd? This won't end well\n");
		return;
	}
	if(initrd_verify((uint32_t*)initrd_startaddr))
	{
		klog(LOG_PANIC,"initrd_init","Valid Initrd Found\n");
	}
	//Populate the tables
	initfs_header = (superheader_t *)initrd_startaddr;
	initfs_fileheaders = (fileheader_t *) (initrd_startaddr+sizeof(superheader_t));
	
	initrd_mountpoint = malloc(sizeof(vfs_node_t));
	memset(initrd_mountpoint, 0x00, sizeof(vfs_node_t));
	strcpy(initrd_mountpoint->name, "ramdisk0");
	initrd_mountpoint->uid = 0;
	initrd_mountpoint->gid = 0;
	initrd_mountpoint->permissions = VFS_PERMISSION_READ;
	initrd_mountpoint->flags   = VFS_DIRECTORY | VFS_MOUNTPOINT;
	initrd_mountpoint->readdir = &initrd_readdir;
	initrd_mountpoint->finddir = &initrd_finddir;
	
	initrd_nodes = (vfs_node_t*)malloc(sizeof(vfs_node_t) * initfs_header->headers);

	for (unsigned int i = 0; i < initfs_header->headers; i++)
	{
		// Edit the file's header - currently it holds the file offset
		// relative to the start of the ramdisk. We want it relative to the start
		// of memory.
		initfs_fileheaders[i].offset += initrd_startaddr;
		// Create a new file node.
		strcpy(initrd_nodes[i].name, &initfs_fileheaders[i].name);
		initrd_nodes[i].length = initfs_fileheaders[i].length;
		initrd_nodes[i].inode = i;
		initrd_nodes[i].flags = VFS_FILE;
		initrd_nodes[i].read = 0; //TODO:IMPLEMENT
		initrd_nodes[i].write = 0;
		initrd_nodes[i].readdir = 0;
		initrd_nodes[i].finddir = 0;
		initrd_nodes[i].open = 0;
		initrd_nodes[i].close = 0;
		
	} 
	 
}