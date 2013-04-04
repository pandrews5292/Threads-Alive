// threadsalive.h

#include "queue.h"

queue* t_queue;

tcb* tcb_init();
void ta_libinit();
void ta_create(void (*)(void), void*);
void ta_yield(void);
int ta_waitall();


