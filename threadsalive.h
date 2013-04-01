// threadsalive.h

#include "queue.h"

void ta_libinit();
void ta_create(void (*)(void), void*);
void ta_yield(void);
//static int ta_waitall();
