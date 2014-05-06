#ifndef STDLIB_H
#define STDLIB_H
#include <types.h>
///Allocates memory
void     *malloc(size_t);				//< The standard function.
///(Re)Allocates memory
void*     realloc(void *p, size_t);		//< The standard function.
void     *calloc(size_t, size_t);		//< The standard function.
///(De)Allocates memory
void      free(void *);					//< The standard function.
#endif
