#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <time.h>
#include <assert.h>

#include "threadsalive.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* GLOBALS */
#define N 10000
#define NPRODUCER 10
#define NCONSUMER 10

talock_t mutex;
tacond_t full;
tacond_t empty;

unsigned int buffer[N];
int nextin = 0, nextout = 0, count = 0;
int stop = FALSE;
unsigned long long pcount = 0ULL;
unsigned long long ccount = 0ULL;

void killerthr(void *v)
{
    time_t finish = time(NULL) + 5;
    while (time(NULL) < finish) ta_yield();
    stop = TRUE;
}

void producer(void *v)
{
    unsigned int iter = 0;
    //fprintf(stderr, "producer thread %d started up\n", ta_self());
    while (!stop) 
    {
	ta_lock(&mutex);
	while (count == N) 
            ta_wait(&full, &mutex);
	count++;
        iter++;
	buffer[nextin] = pcount++;
	nextin = (nextin + 1) % N;
        ta_yield();
	ta_signal(&empty);
	ta_unlock(&mutex);
    }
    //fprintf(stderr, "producer thread %d ran through %d iterations\n", ta_self(), iter);
}

void consumer(void *v)
{
    unsigned int iter = 0;
    //fprintf(stderr, "consumer thread %d started up\n", ta_self());
    while (!stop) 
    {
	ta_lock(&mutex);
	while (count == 0) 
            ta_wait(&empty, &mutex);
	assert(buffer[nextout] == ccount);
        ccount++;
	nextout = (nextout + 1) % N;
	count--;
        iter++;
        ta_yield();
	ta_signal(&full);
	ta_unlock(&mutex);
    }
    //fprintf(stderr, "consumer thread %d ran through %d iterations\n", ta_self(), iter);
}


int main(int argc, char **argv)
{
    ta_libinit();

    ta_lock_init(&mutex);
    ta_cond_init(&full);
    ta_cond_init(&empty);

    int i;
    for (i = 0; i < N; i++)
        buffer[i] = 0xFEEDFEED;

    ta_create(killerthr, NULL);

    for (i = 0; i < NPRODUCER; i++)
    {
        ta_create(producer, NULL);
    }

    for (i = 0; i < NCONSUMER; i++)
    {
        ta_create(consumer, NULL);
    }

    int rv = ta_waitall();
    assert(rv == 0);

    ta_lock_destroy(&mutex);
    ta_cond_destroy(&full);
    ta_cond_destroy(&empty);

    return 0;
}

