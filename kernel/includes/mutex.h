#ifndef MUTEX_H
#define MUTEX_H
#include <types.h>
/**
A binary semaphore
**/
typedef volatile uint8_t mutex_t;
/**
Locks the mutex
If it cannot lock it, it spins until it is freed.
**/
void mutex_lock(mutex_t *mutex);
/**
Unlocks the mutex.
Does nothing if the mutex is unlocked
**/
void mutex_unlock(mutex_t *mutex);
/**
Initialises a mutex. Call per mutex
**/
void mutex_init(mutex_t *mutex);
#endif
