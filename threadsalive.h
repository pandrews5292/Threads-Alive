// threadsalive.h
#ifndef THREADSALIVE_H
#define THREADSALIVE_H

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "sema_queue.h"

static struct queue* t_queue;

//Create a semaphore
void ta_sem_init(tasem_t *sema, int value);
//Destroy a semaphore
void ta_sem_destroy(tasem_t *sema);
//Signal a thread waiting on the semaphore
void ta_sem_signal(tasem_t *sema);
//Wait for a semaphore to be free
void ta_sem_wait(tasem_t *sema);

//Create a lock
void ta_lock_init(talock_t *lock);
//Destroy a lock
void ta_lock_destroy(talock_t *lock);
//Obtain the lock
void ta_lock(talock_t *lock);
//Release the lock
void ta_unlock(talock_t *lock);

//Create a cond var
void ta_cond_init(tacond_t*);
//Destroy a cond var
void ta_cond_destroy(tacond_t*);
//Enter critical section; wait for cond var and release lock
void ta_wait(tacond_t*, talock_t*);
//Signal a thread to stop waiting on cond var
void ta_signal(tacond_t*);

//Create a thread control block. Context automatically set to current context
tcb* tcb_init();

//Run before using threadsalive library
void ta_libinit();
//Create a thread
void ta_create(void (*)(void*), void*);
//Yield CPU to another thread
void ta_yield(void);
//Wait for all threads to finish; acts as the scheduler
int ta_waitall();



#endif
