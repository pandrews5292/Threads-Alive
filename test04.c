#include <stdio.h>
#include <stdlib.h>
#include "threadsalive.h"

talock_t mutex;

int tid = 0;

void threadfn(void *arg)
{
    int *i = (int *)arg;

    ta_lock(&mutex);
    int mytid = tid++;
    fprintf(stderr, "thread %d has lock\n", mytid);
    *i += 100; 
    fprintf(stderr, "thread %d yielding processor with lock\n", mytid);
    ta_yield();
    
    fprintf(stderr, "thread %d leaving mutex in locked state... one thread will block indefinitely\n", mytid);
}

int main(int argc, char **argv)
{
    ta_libinit();
    int i = 0;

    ta_lock_init(&mutex);

    ta_create(threadfn, (void *)&i);
    ta_create(threadfn, (void *)&i);

    int rv = ta_waitall();
    if (rv)
    {
        fprintf(stderr, "%d threads were not ready to run when ta_waitall() was called\n", -rv);
    }

    ta_lock_destroy(&mutex);

    return 0;
}
