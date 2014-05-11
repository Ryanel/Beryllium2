#include <beryllium/thread.h>
#include <lib/list.h>
#include <log.h>
typedef struct tlist {
	struct tlist * next;
	thread_t * thread;
} tlist_t;

volatile tlist_t * tqueue;
volatile tlist_t * tcurrent;
int scheduler_state = 0; //Do nothing
void scheduler_update()
{

}

void scheduler_add_thread(thread_t * thread)
{
	tlist_t * thread_item = malloc(sizeof(tlist_t));
	
	thread_item->thread = thread;
	
	tlist_t *test = tqueue;
	
	while (test->next)
		//test = test->next;
	test->next = thread_item;
	scheduler_state = 1;
}

void scheduler_init(thread_t * kernel_thread)
{
	klog(LOG_WARN,"scheduler_init","Starting scheduler...\n");
	//tqueue = malloc(sizeof(tlist_t));
	//tqueue->thread = kernel_thread;
	//tcurrent = tqueue;
	timing_register_timer("scheduler_update",5,scheduler_update, 5);
}