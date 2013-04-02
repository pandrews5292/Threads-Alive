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
