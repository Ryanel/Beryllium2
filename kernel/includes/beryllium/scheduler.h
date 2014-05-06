#ifndef SCHED_H
#define SCHED_H
#include <beryllium/thread.h>
#include <types.h>
void scheduler_init(thread_t *initial_thread);
void scheduler_add(thread_t *thread);
void scheduler_remove();
void scheduler_block(); ///TODO: Implement
void scheduler_unblock(); ///TODO: Implement

/// Linked list of threads

typedef struct thread_list
{
	thread_t *thread;
	struct thread_list *next;
} thread_list_t;

#endif
