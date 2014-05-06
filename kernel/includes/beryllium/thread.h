#ifndef THREAD_T
#define THREAD_T

#include <types.h>

typedef volatile struct
{
	uint32_t esp,ebp,ebx,esi,edi,eflags;
	uint32_t id;
} thread_t;

#include <beryllium/scheduler.h>

thread_t *threading_init();
thread_t *thread_create (int (*fn)(void*), void *arg, uint32_t *stack);
thread_t *thread_create_named (int (*fn)(void*), void *arg, uint32_t *stack, char* name);
//void thread_switch(thread_list_t *to);
#endif
