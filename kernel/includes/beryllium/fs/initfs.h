#ifndef INITFS_H
#define INITFS_H
typedef struct {
	char 		magic_a; //I
	char 		magic_b; //N
	char 		magic_c; //I
	char 		magic_d; //T 4
	uint32_t 	headers;   // 8
	uint32_t    checksum;// A + B + C + D + files
	uint8_t     pad[52];
} superheader_t;

typedef struct
{
   uint32_t magic;     // Magic number, for error checking.
   char name[52];  // Filename.
   uint32_t offset;   // Offset in the initrd that the file starts.
   uint32_t length;   // Length of the file.
} fileheader_t;
#endif