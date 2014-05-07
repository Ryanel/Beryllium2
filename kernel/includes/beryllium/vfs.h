#ifndef VFS_H
#define VFS_H
#include <types.h>
#include <lib/tree.h>
#define VFS_FILE        (1 << 0)
#define VFS_DIRECTORY   (1 << 1)
#define VFS_CHARDEVICE  (1 << 2)
#define VFS_BLOCKDEVICE (1 << 3)
#define VFS_PIPE        (1 << 4)
#define VFS_SYMLINK     (1 << 5)
#define VFS_MOUNTPOINT  (1 << 6)

#define VFS_PERMISSION_READ  0x01
#define VFS_PERMISSION_WRITE 0x02
#define VFS_PERMISSION_EXECUTE 0x04

#define VFS_DEVICE_VFS  (1 << 0)
#define VFS_DEVICE_RAMDISK (1 << 1)

#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STRING "/"
#define PATH_UP  ".."
#define PATH_DOT "."
struct vfs_node;
extern tree_t     *vfs_tree;
struct dirent // One of these is returned by the readdir call, according to POSIX.
{
  char name[128]; // Filename.
  uint32_t ino;     // Inode number. Required by POSIX.
};

typedef uint32_t (*read_type_t) (struct vfs_node *, uint32_t, uint32_t, uint8_t *);
typedef uint32_t (*write_type_t) (struct vfs_node *, uint32_t, uint32_t, uint8_t *);
typedef void (*open_type_t) (struct vfs_node *, unsigned int flags);
typedef void (*close_type_t) (struct vfs_node *);
typedef void (*ioctl_type_t) (struct vfs_node *);
typedef struct dirent *(*readdir_type_t) (struct vfs_node *, uint32_t);
typedef struct vfs_node *(*finddir_type_t) (struct vfs_node *, char *name);
typedef void (*create_type_t) (struct vfs_node *, char *name, uint16_t permission);

typedef struct vfs_node{
	char name[256];
	uint32_t type;
	uint32_t flags;
	uint32_t permissions;
	uint32_t uid;
	uint32_t gid;
	uint32_t inode;
	uint32_t length;
	uint32_t device; ///Internal device number
	uint32_t device_type;
	//File Functions
	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	ioctl_type_t ioctl;
	readdir_type_t readdir;
	finddir_type_t finddir;
	create_type_t create;
	struct vfs_node *ptr; // Used by mountpoints and symlinks.
} vfs_node_t;

struct vfs_entry {
	char * name;
	vfs_node_t * file;
};

vfs_node_t* vfs_getRootNode();
void vfs_printnode(vfs_node_t *node);
void vfs_init();

uint32_t read_vfs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_vfs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_vfs(vfs_node_t *node, unsigned int flags);
void close_vfs(vfs_node_t *node);

char *canonicalize_path(char *cwd, char *input);
vfs_node_t *kopen(char *filename, uint32_t flags);
int vfs_mount(char * path, vfs_node_t * local_root);
vfs_node_t *get_mount_point(char * path, unsigned int path_depth, char **outpath, unsigned int * outdepth);
int create_file_vfs(char *name, uint16_t permission);
void vfs_print_tree_node(tree_node_t * node, size_t height);

#endif
