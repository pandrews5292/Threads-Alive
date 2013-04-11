#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "threadsalive.h"

talock_t *locks = NULL;
int *data = NULL;

void thread1(void *arg)
{
    int tid = (int)arg;
    talock_t *lock = &locks[tid];
    fprintf(stderr, "lock id in thread %d is %d\n", tid, locks[tid]);
    ta_lock(lock);
    ta_yield();
    data[tid] += 42;
    ta_yield();
//     ta_unlock(lock);
}

void thread2(void *arg)
{
    int tid = (int)arg;
    talock_t *lock = &locks[tid];
    fprintf(stderr, "lock id in thread %d is %d\n", tid, locks[tid]);
    ta_lock(lock);
    ta_yield();
    data[tid] -= 42;
    ta_yield();
    ta_unlock(lock);
}

int main(int argc, char **argv)
{
    srandom(time(NULL));

    ta_libinit();
    int i = 0;
    int nthreads = 2;//random() % 100 + 1;

    locks = (talock_t *)malloc(sizeof(talock_t) * nthreads);
    data = (int *)malloc(sizeof(int) * nthreads);

    assert(locks);
    assert(data);

    memset(data, 0, sizeof(int)*nthreads);

    for (i = 0; i < nthreads; i++)
    {
        ta_lock_init(&locks[i]);
        fprintf(stderr, "lock id for lock %d is %d\n", i, locks[i]);
    }

    for (i = 0; i < nthreads; i++)
    {
        ta_create(thread1, (void *)i);
        ta_create(thread2, (void *)i);
    }

    int rv = ta_waitall();
    assert(rv < 0);

    for (i = 0; i < nthreads; i++)
    {
        assert(data[i] == 42);
    }

    for (i = 0; i < nthreads; i++)
    {
        ta_lock_destroy(&locks[i]);
    }


    free(locks);
    free(data);

    return 0;
}
