#include <types.h>
#include <log.h>
#include <stdio.h>
#include <beryllium/fs/initfs.h>
#include <beryllium/vfs.h>
#include <lib/tree.h>
uint32_t initrd_exists = 0;
uint32_t initrd_startaddr = 0;
uint32_t initrd_endaddr = 0;

superheader_t * initfs_header;
fileheader_t  * initfs_fileheaders;

vfs_node_t  * initrd_mountpoint;
vfs_node_t  * initrd_nodes;
vfs_entry_t * initrd_entries;
void initrd_set_location(uint32_t start, uint32_t end)
{
	initrd_startaddr	= start;
	initrd_endaddr		= end;
	initrd_exists		= 1;
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
	strcpy(initrd_mountpoint->name, "ramdisk");
	initrd_mountpoint->uid = 0;
	initrd_mountpoint->gid = 0;
	initrd_mountpoint->permissions = VFS_PERMISSION_READ;
	initrd_mountpoint->flags   = VFS_DIRECTORY;
	initrd_mountpoint->readdir = NULL;
	initrd_mountpoint->finddir = NULL;
	
	initrd_nodes = (vfs_node_t*)malloc(sizeof(vfs_node_t) * initfs_header->headers);
	
	initrd_entries = (vfs_entry_t*)malloc(sizeof(vfs_entry_t) * initfs_header->headers);
	
	for (int i = 0; i < initfs_header->headers; i++)
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
		initrd_nodes[i].read = 0;//&initrd_read;
		initrd_nodes[i].write = 0;
		initrd_nodes[i].readdir = 0;
		initrd_nodes[i].finddir = 0;
		initrd_nodes[i].open = 0;
		initrd_nodes[i].close = 0;
		initrd_entries[i].file = &initrd_nodes[i];
		strcpy(initrd_entries[i].name, &initfs_fileheaders[i].name);
		tree_node_insert_child(vfs_tree, vfs_tree->root, &initrd_entries[i]); //Insert the node into the tree
		
		printf("Created file /%s\n",initrd_entries[i].name);
		
	} 
	
	vfs_mount("/",initrd_mountpoint);
	vfs_print_tree_node(vfs_tree->root, 0);
	 
}