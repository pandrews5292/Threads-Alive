// ta_mutex.h

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

typedef struct {
    int value;
    queue* w_queue;
} tasem_t;

typedef struct {
    tasem_t* sem; 
}talock_t;

void ta_sem_init(tasem_t *sema, int value);
void ta_sem_destroy(tasem_t *sema);
void ta_sem_signal(tasem_t *sema);
void ta_sem_wait(tasem_t *sema);

void ta_lock_init(talock_t *lock);
void ta_lock_destroy(talock_t *lock);
void ta_lock(talock_t *lock);
void ta_unlock(talock_t *lock);

void cas(int*, int, int);
