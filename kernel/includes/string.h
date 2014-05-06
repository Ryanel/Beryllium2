#ifndef STRING_H
#define STRING_H
#include <types.h>
///Compares two strings
int strcmp (const char *str1,const char *str2);
///Length of a string
int strlen(const char *str);
///Copies string src into dest
char * strcpy(char * dest, const char * src);
///Copies src(n) to dest
void *memcpy(void *dest,const void *src,size_t n);
///Sets dest to val n times
void *memset(void *dest,int val,size_t n);
///Moves n src to dest
void memmove(void *dest, const void *src, size_t n);

char *strdup(char *s1);
char * strpbrk(const char * str, const char * accept);
size_t lfind(const char * str, const char accept);
size_t strspn(const char * str, const char * accept);
char * strtok_r(char * str, const char * delim, char ** saveptr);
#endif
