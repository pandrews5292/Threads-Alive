// threadsalive.h
#ifndef THREADSALIVE_H
#define THREADSALIVE_H

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "sema_queue.h"

static struct queue* t_queue;

void ta_sem_init(tasem_t *sema, int value);
void ta_sem_destroy(tasem_t *sema);
void ta_sem_signal(tasem_t *sema);
void ta_sem_wait(tasem_t *sema);

void ta_lock_init(talock_t *lock);
void ta_lock_destroy(talock_t *lock);
void ta_lock(talock_t *lock);
void ta_unlock(talock_t *lock);

void ta_cond_init(tacond_t*);
void ta_cond_destroy(tacond_t*);
void ta_wait(tacond_t*, talock_t*);
void ta_signal(tacond_t*);

tcb* tcb_init();
void ta_libinit();
void ta_create(void (*)(void*), void*);
void ta_yield(void);
int ta_waitall();



#endif
