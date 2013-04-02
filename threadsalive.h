// threadsalive.h

#include "queue.h"

void ta_libinit();
void ta_create(void (*)(void), void*);
void ta_yield(void);
int ta_waitall();
