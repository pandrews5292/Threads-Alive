// ta_mutex.c

#include "ta_mutex.h"

static int guard = 0;

void ta_sem_init(tasem_t* sema, int value) {
  sema = (tasem_t*)malloc(sizeof(tasem_t));
  sema->value = value;
  sema->w_queue = create_queue();
}

void ta_sem_destroy(tasem_t* sema) {
  destroy_queue(sema->w_queue);
  free(sema);
}

void ta_sem_signal(tasem_t* sema) {
    while(cas(&guard, 0, 1));
  
}

void ta_sem_wait(tasem_t* sema){
    while(!cas(&guard, 0, 1));
    sema->value--;
    if (sema->value < 0){
	tcb* tcb = tcb_init();
	sema->w_queue->push(tcb);
	//block()
	guard = 0;
    }
    else
	guard = 0;
}

void ta_lock_init(talock_t* lock){
    lock = (talock_t*)malloc(sizeof(talock_t));
    ta_sem_init(lock->sem, 1);
}

void ta_lock_destroy(talock_t* lock){
    ta_sem_destroy(lock->sem);
    free(lock);
}

void ta_lock(talock_t* lock){
    ta_sem_wait(lock->sem);
}

void ta_unlock(talock_t* lock){
    ta_sem_signal(lock->sem);
}

int cas(int* ptr, int old, int new) {
  unsigned char ret;

  __asm__ __volatile__ (
	"  lock\n"
	"  cmpxchgl %2,%1\n"
	"  sete %0\n"
	: "=q" (ret), "=m" (*ptr)
        : "r" (new), "m" (*ptr), "a" (void)
        : "memory");
  return ret;
}
