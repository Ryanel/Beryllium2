#include <beryllium/thread.h>
#include <stdlib.h>
#include <string.h>
#include <log.h>
uint32_t thread_id = 0;

thread_t * current_thread;
thread_t * kmain_thread;

void thread_exit()
{
	klog(LOG_WARN,"thread_exit","Thread exiting!\n");
	while(true)
	{
		
	}
}

thread_t * thread_create(uint8_t level,uint32_t pid, int (*fn)(void*))
{
	thread_t * thread = (thread_t *)malloc(sizeof(thread_t));
	memset (thread, 0, sizeof (thread_t));
	thread->tid = thread_id++;
	thread->level = level;
	thread->owner_pid = pid;
	thread->stack = malloc(0x1000);
	uint32_t * stack_ptr = thread->stack + 0x500;
	*--stack_ptr = (uint32_t)&thread_exit; // Fake return address.
	*--stack_ptr = (uint32_t)&fn;
	*--stack_ptr = 0; // Fake EBP.

	thread->ebp = (uint32_t)stack_ptr;
	thread->esp = (uint32_t)stack_ptr;
	return thread;
}

thread_t *threading_start()
{
	klog(LOG_WARN,"threading_start","Starting threading...\n");
	thread_t *thread = malloc (sizeof (thread_t));
	thread->tid = thread_id++;
	current_thread = thread;
	return thread;
}
void thread_switch (thread_t *next)
{
	asm volatile ("mov %%esp, %0" : "=r" (current_thread->esp));
	asm volatile ("mov %%ebp, %0" : "=r" (current_thread->ebp));
	asm volatile ("mov %%ebx, %0" : "=r" (current_thread->ebx));
	asm volatile ("mov %%esi, %0" : "=r" (current_thread->esi));
	asm volatile ("mov %%edi, %0" : "=r" (current_thread->edi));
	asm volatile ("mov %0, %%edi" : : "r" (next->edi));
	asm volatile ("mov %0, %%esi" : : "r" (next->esi));
	asm volatile ("mov %0, %%ebx" : : "r" (next->ebx));
	asm volatile ("mov %0, %%esp" : : "r" (next->esp));
	asm volatile ("mov %0, %%ebp" : : "r" (next->ebp));
}

void thread_switchkernel()
{
	klog(LOG_WARN,"thread_switchkernel","Switching back to kernel thread... (UNIMPLEMENTED)\n");
}
