#ifndef TYPE_H
#define TYPE_H
#ifdef B64
typedef unsigned long long  uintptr_t;
#else
typedef unsigned int		uintptr_t;
#endif

typedef   signed long long  sint64_t;
typedef unsigned long long  uint64_t;
typedef unsigned int   uint32_t;
typedef          int   sint32_t;
typedef unsigned short uint16_t;
typedef          short sint16_t;
typedef unsigned char  uint8_t;
typedef          char  sint8_t;

typedef unsigned int  size_t;
typedef void* type_t;

typedef uint8_t bool;
#define true 1
#define false 0
#ifndef NULL
#define NULL (void*)0
#endif
#endif
