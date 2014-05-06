#ifndef TIMING_H
#define TIMING_H
typedef struct timer_entry {
	char *name;
    int fire_tick;
    int repeat_rate;
    int no;
    void (*handler)(struct timer_entry *);
} timer_t;
int timing_init();
int timing_register_timer(char *name,int tick,void (*handler)(timer_t *),int reschedule);
void timing_deregister_timer(void (*handler)(timer_t *));
void timing_remove_index(int index);
void list_timers();
#endif
