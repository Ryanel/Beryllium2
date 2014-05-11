#include <beryllium/thread.h>
#include <lib/list.h>
#include <log.h>
typedef struct thread_list {
	struct thread_list * next;
	thread_t * thread;
} tlist_t;

volatile tlist_t * tqueue = NULL;
volatile tlist_t * tcurrent = NULL;

int scheduler_state = 0; //Do nothing
void scheduler_update()
{
	if(!scheduler_state)
	{
		return;
	}
	
	tcurrent = tcurrent->next;
	if(tcurrent == NULL)
	{
		tcurrent = tqueue;
	}
	printf("thread: Launching 0x%X\n",tcurrent->thread);
	thread_switch(tcurrent->thread);
}

void scheduler_add_thread(thread_t * thread)
{
	tlist_t * thread_item = (tlist_t*)malloc(sizeof(tlist_t));
	
	thread_item->thread = thread;
	thread_item->next = NULL;
	
	tlist_t *test = tqueue;
	
	while (true)
	{
		printf("tester: 0x%X\n",test);
		if(test->next)
		{
			printf("Advancing...\n");
			test = test->next;
		}
		else
		{
			break;
		}
	}
	test->next = thread_item;
}

void scheduler_init(thread_t * kernel_thread)
{
	klog(LOG_WARN,"scheduler_init","Starting scheduler...\n");
	tqueue = (tlist_t*)malloc(sizeof(tlist_t));
	tqueue->next = NULL;
	tqueue->thread = kernel_thread;
	tcurrent = tqueue;
	scheduler_state = 1;
	timing_register_timer("scheduler_update",5,scheduler_update, 5);
}
