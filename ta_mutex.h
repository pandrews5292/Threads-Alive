// ta_mutex.h

#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#inculde "queue.h"

typedef struct {
  int counter;
  queue* waitlist;
} tasem_t;

typedef struct {
  tasem_t* sem;
} talock_t;

void ta_sem_init(
