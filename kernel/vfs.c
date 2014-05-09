#include <beryllium/vfs.h>
#include <string.h>
#include <log.h>
#include <lib/tree.h>
#include <stdlib.h>
#include <stdio.h>
#include <beryllium/driver.h>
#include <beryllium/device.h>
driver_t			 vfs_driver;
device_t            *vfs_device;
tree_t     			*vfs_tree 		= NULL;
struct vfs_entry 	*vfs_root 		= NULL;
vfs_node_t 			*vfs_root_node 	= NULL;

void vfs_init()
{
	vfs_tree = tree_create();

	vfs_root = malloc(sizeof(struct vfs_entry));
	vfs_root->name = strdup("[root]");
	vfs_root->file =  NULL;
	tree_set_root(vfs_tree, vfs_root);
	return 0;
}

uint32_t read_vfs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) 
{
	if (node->read) {
		uint32_t ret = node->read(node, offset, size, buffer);
		return ret;
	} else {
		return 0;
	}
}
uint32_t write_vfs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) 
{
	if (node->write) {
		uint32_t ret = node->write(node, offset, size, buffer);
		return ret;
	} else {
		return 0;
	}
}
void open_vfs(vfs_node_t *node, unsigned int flags) 
{
	printf("open_vfs: node: 0x%X\n",node);
	if (node->open) {
		node->open(node, flags);
	}
}

void ioctl_vfs(vfs_node_t *node) 
{
	printf("ioctl_vfs: node: 0x%X\n",node);
	if (node->ioctl) {
		node->ioctl(node);
	}
}

void close_vfs(vfs_node_t *node) 
{
	if(node == vfs_root_node)
	{
		klog(LOG_SEVERE,"VFS","Kernel just attempted to close the root filesystem. Disallowing...\n");
		return;
	}
	if (node->close) {
		node->close(node);
	}
}

struct dirent *readdir_vfs (vfs_node_t *node, uint32_t index)
{
	if ((node->type & VFS_DIRECTORY) && node->readdir)
	{
		struct dirent *ret = node->readdir(node, index);
		return ret;
	}
	else
	{
		return (struct dirent *)NULL;
	}
}

vfs_node_t *finddir_vfs(vfs_node_t *node, char *name)
{
	if ((node->type & VFS_DIRECTORY) && node->finddir)
	{
		vfs_node_t *ret = node->finddir(node, name);
		return ret;
	} 
	else
	{
		printf("finddir not supported\n");
		return (vfs_node_t *)NULL;
	}
}

int create_file_vfs(char *name, uint16_t permission)
{
	vfs_node_t * parent;
	char *cwd = "/";
	char *path = canonicalize_path(cwd, name);

	char * parent_path = malloc(strlen(path) + 4);
	sprintf(parent_path, "%s", path);

	char * f_path = path + strlen(path) - 1;
	while (f_path > path) {
		if (*f_path == '/') {
			f_path += 1;
			break;
		}
		f_path--;
	}

	klog(LOG_WARN,"VFS","creating file %s within %s (hope these strings are good)\n", f_path, parent_path);

	parent = kopen(parent_path, 0);
	
	free(parent_path);
	if (!parent) {
		printf("No parent to file\n");
		//free(path);
		return -1;
	}
	
	if (parent->create) {
		parent->create(parent, f_path, permission);
	}
	else
	{
		printf("No create function for parent!\n");
	}
	free(path);
	free(parent);

	return 0;
}

/*
Credit Kevin Lange AGAIN for the path canonicalizer
*/
char *canonicalize_path(char *cwd, char *input) {
	/* This is a stack-based canonicalizer; we use a list as a stack */
	list_t *out = list_create();

	/*
	 * If we have a relative path, we need to canonicalize
	 * the working directory and insert it into the stack.
	 */
	if (strlen(input) && input[0] != PATH_SEPARATOR) {
		/* Make a copy of the working directory */
		char *path = malloc((strlen(cwd) + 1) * sizeof(char));
		memcpy(path, cwd, strlen(cwd) + 1);

		/* Setup tokenizer */
		char *pch;
		char *save;
		pch = strtok_r(path,PATH_SEPARATOR_STRING,&save);

		/* Start tokenizing */
		while (pch != NULL) {
			/* Make copies of the path elements */
			char *s = malloc(sizeof(char) * (strlen(pch) + 1));
			memcpy(s, pch, strlen(pch) + 1);
			/* And push them */
			list_insert(out, s);
			pch = strtok_r(NULL,PATH_SEPARATOR_STRING,&save);
		}
		free(path);
	}

	/* Similarly, we need to push the elements from the new path */
	char *path = malloc((strlen(input) + 1) * sizeof(char));
	memcpy(path, input, strlen(input) + 1);

	/* Initialize the tokenizer... */
	char *pch;
	char *save;
	pch = strtok_r(path,PATH_SEPARATOR_STRING,&save);

	/*
	 * Tokenize the path, this time, taking care to properly
	 * handle .. and . to represent up (stack pop) and current
	 * (do nothing)
	 */
	while (pch != NULL) {
		if (!strcmp(pch,PATH_UP)) {
			/*
			 * Path = ..
			 * Pop the stack to move up a directory
			 */
			list_node_t * n = list_pop(out);
			if (n) {
				free(n->value);
				free(n);
			}
		} else if (!strcmp(pch,PATH_DOT)) {
			/*
			 * Path = .
			 * Do nothing
			 */
		} else {
			/*
			 * Regular path, push it
			 * XXX: Path elements should be checked for existence!
			 */
			char * s = malloc(sizeof(char) * (strlen(pch) + 1));
			memcpy(s, pch, strlen(pch) + 1);
			list_insert(out, s);
		}
		pch = strtok_r(NULL, PATH_SEPARATOR_STRING, &save);
	}
	free(path);

	/* Calculate the size of the path string */
	size_t size = 0;
	foreach(item, out) {
		/* Helpful use of our foreach macro. */
		size += strlen(item->value) + 1;
	}

	/* join() the list */
	char *output = malloc(sizeof(char) * (size + 1));
	char *output_offset = output;
	if (size == 0) {
		/*
		 * If the path is empty, we take this to mean the root
		 * thus we synthesize a path of "/" to return.
		 */
		output = realloc(output, sizeof(char) * 2);
		output[0] = PATH_SEPARATOR;
		output[1] = '\0';
	} else {
		/* Otherwise, append each element together */
		foreach(item, out) {
			output_offset[0] = PATH_SEPARATOR;
			output_offset++;
			memcpy(output_offset, item->value, strlen(item->value) + 1);
			output_offset += strlen(item->value);
		}
	}

	/* Clean up the various things we used to get here */
	list_destroy(out);
	list_free(out);
	free(out);

	/* And return a working, absolute path */
	return output;
}

#include <stdio.h>
void vfs_print_tree_node(tree_node_t * node, size_t height)
{
	/* End recursion on a blank entry */
	if (!node) return;
	/* Indent output */
	for (uint32_t i = 0; i < height; ++i) { printf(" "); }
	/* Get the current process */
	struct vfs_entry * fnode = (struct vfs_entry *)node->value;
	/* Print the process name */
	if (fnode->file) {
		printf("%s -> 0x%x (%s)", fnode->name, fnode->file, fnode->file->name);
	} else {
		printf("%s -> (no file data)", fnode->name);
	}
	/* Linefeed */
	printf("\n");
	foreach(child, node->children) {
		/* Recursively print the children */
		vfs_print_tree_node(child->value, height + 4);
	}
}
int vfs_mount(char * path, vfs_node_t * local_root) {
	if (!vfs_tree) {
		klog(LOG_SEVERE,"VFS", "VFS hasn't been initialized, you can't mount things yet!\n");
		return 1;
	}
	if (!path || path[0] != '/') {
		klog(LOG_ERROR,"VFS", "Path must be absolute for mountpoint.\n");
		return 2;
	}
	int ret_val = 0;

	char * p = strdup(path);
	char * i = p;

	int path_len   = strlen(p);

	/* Chop the path up */
	while (i < p + path_len) {
		if (*i == PATH_SEPARATOR) {
			*i = '\0';
		}
		i++;
	}
	/* Clean up */
	p[path_len] = '\0';
	i = p + 1;

	/* Root */
	tree_node_t * root_node = vfs_tree->root;

	if (*i == '\0') {
		/* Special case, we're trying to set the root node */
		struct vfs_entry * root = (struct vfs_entry *)root_node->value;
		if (root->file) {
			klog(LOG_WARN,"VFS", "Path %s already mounted, unmount before trying to mount something else.\n", path);
			ret_val = 3;
			goto _vfs_cleanup;
		}
		root->file = local_root;
		/* We also keep a legacy shortcut around for that */
		vfs_root_node = local_root;
	} else {

		tree_node_t * node = root_node;
		char * at = i;
		while (1) {
			if (at >= p + path_len) {
				break;
			}
			int found = 0;
			//klog(LOG_INFO,"VFS", "Searching for %s\n", at);
			foreach(child, node->children) {
				tree_node_t * tchild = (tree_node_t *)child->value;
				struct vfs_entry * ent = (struct vfs_entry *)tchild->value;
				if (!strcmp(ent->name, at)) {
					found = 1;
					node = tchild;
					break;
				}
			}
			if (!found) {
				//klog(LOG_INFO,"VFS", "Did not find %s, making it.\n", at);
				struct vfs_entry * ent = malloc(sizeof(struct vfs_entry));
				ent->name = strdup(at);
				ent->file = NULL;
				node = tree_node_insert_child(vfs_tree, node, ent);
			}
			at = at + strlen(at) + 1;
		}
		struct vfs_entry * ent = (struct vfs_entry *)node->value;
		if (ent->file) {
			klog(LOG_WARN,"VFS", "Path %s already mounted, unmount before trying to mount something else.\n", path);
			ret_val = 3;
			goto _vfs_cleanup;
		}
		ent->file = local_root;
		//vfs_print_node(local_root);
	}

_vfs_cleanup:
	free(p);
	return ret_val;
}

vfs_node_t *get_mount_point(char * path, unsigned int path_depth, char **outpath, unsigned int * outdepth) {
	size_t depth;

	for (depth = 0; depth <= path_depth; ++depth) {
		path += strlen(path) + 1;
	}

	/* Last available node */
	vfs_node_t   * last = vfs_root_node;
	tree_node_t * node = vfs_tree->root;

	char * at = *outpath;
	int _depth = 1;
	int _tree_depth = 0;

	while (1) {
		if (at >= path) {
			break;
		}
		int found = 0;
		//klog(LOG_INFO,"VFS","Searching for %s\n", at);
		foreach(child, node->children) {
			tree_node_t * tchild = (tree_node_t *)child->value;
			struct vfs_entry * ent = (struct vfs_entry *)tchild->value;
			if (!strcmp(ent->name, at)) {
				found = 1;
				node = tchild;
				at = at + strlen(at) + 1;
				if (ent->file) {
					_tree_depth = _depth;
					last = ent->file;
					*outpath = at;
				}
				break;
			}
		}
		if (!found) {
			break;
		}
		_depth++;
	}

	*outdepth = _tree_depth;

	if (last) {
		vfs_node_t * last_clone = malloc(sizeof(vfs_node_t));
		memcpy(last_clone, last, sizeof(vfs_node_t));
		return last_clone;
	}
	return last;
}

vfs_node_t *kopen(char *filename, uint32_t flags) {
	/* Simple sanity checks that we actually have a file system */
	if (!vfs_tree->root || !filename) {
		printf("vfs_tree->root or filename = NULL!\n");
		return NULL;
	}

	klog(LOG_INFO,"kopen", "Opening %s\n", filename);

	/* Reference the current working directory */
	char *cwd = "/";
	/* Canonicalize the (potentially relative) path... */
	char *path = canonicalize_path(cwd, filename);
	/* And store the length once to save recalculations */
	size_t path_len = strlen(path);

	/* If strlen(path) == 1, then path = "/"; return root */
	if (path_len == 1) {
		/* Clone the root file system node */
		vfs_node_t *root_clone = malloc(sizeof(vfs_node_t));
		memcpy(root_clone, vfs_root_node, sizeof(vfs_node_t));

		/* Free the path */
		free(path);

		/* And return the clone */
		return root_clone;
	}

	/* Otherwise, we need to break the path up and start searching */
	char *path_offset = path;
	uint32_t path_depth = 0;
	while (path_offset < path + path_len) {
		/* Find each PATH_SEPARATOR */
		if (*path_offset == PATH_SEPARATOR) {
			*path_offset = '\0';
			path_depth++;
		}
		path_offset++;
	}
	/* Clean up */
	path[path_len] = '\0';
	path_offset = path + 1;

	/*
	 * At this point, the path is tokenized and path_offset points
	 * to the first token (directory) and path_depth is the number
	 * of directories in the path
	 */

	/*
	 * Dig through the (real) tree to find the file
	 */
	unsigned int depth = 0;
	vfs_node_t *node_ptr = malloc(sizeof(vfs_node_t));
	/* Find the mountpoint for this file */
	vfs_node_t *mount_point = get_mount_point(path, path_depth, &path_offset, &depth);

	if (path_offset >= path+path_len) {
		free(path);
		return mount_point;
	}
	/* Set the active directory to the mountpoint */
	memcpy(node_ptr, mount_point, sizeof(vfs_node_t));
	vfs_node_t *node_next = NULL;
	for (; depth < path_depth; ++depth) {
		/* Search the active directory for the requested directory */
		//klog(LOG_INFO,"VFS","... Searching for %s\n", path_offset);
		node_next = finddir_vfs(node_ptr, path_offset);
		free(node_ptr);
		node_ptr = node_next;
		if (!node_ptr) {
			/* We failed to find the requested directory */
			free((void *)path);
			return NULL;
		} else if (depth == path_depth - 1) {
			/* We found the file and are done, open the node */
			open_vfs(node_ptr, flags);
			free((void *)path);
			return node_ptr;
		}
		/* We are still searching... */
		path_offset += strlen(path_offset) + 1;
	}
	//klog(LOG_INFO,"VFS","- Not found.");
	/* We failed to find the requested file, but our loop terminated. */
	free((void *)path);
	return NULL;
}
