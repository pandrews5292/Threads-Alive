#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "threadsalive.h"

void thread1(void *arg)
{
    int rounds = random() % 20;
    int *i = (int *)arg;
    *i += 7;
    fprintf(stderr, "begin t1: %d\n", *i);

    for ( ; rounds > 0; rounds--)
    {
        *i += 1;
        ta_yield();
    }

    *i += 7;
    fprintf(stderr, "end t1: %d\n", *i);
}

void thread2(void *arg)
{
    int rounds = random() % 20;

    int *i = (int *)arg;
    *i -= 7;
    fprintf(stderr, "begin t2: %d\n", *i);

    for ( ; rounds > 0; rounds--)
    {
        *i += 1;
        ta_yield();
    }

    *i -= 7;
    fprintf(stderr, "end t2: %d\n", *i);
}

int main(int argc, char **argv)
{
    srandom(time(NULL));

    ta_libinit();
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        ta_create(thread1, (void *)&i);
        ta_create(thread2, (void *)&i);
    }

    int rv = ta_waitall();
    if (rv)
    {
        fprintf(stderr, "%d threads were not ready to run when ta_waitall() was called\n", -rv);
    }
    return 0;
}
