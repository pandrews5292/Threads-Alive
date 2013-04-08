#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <errno.h>

#include "threadsalive.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define BUFFERSIZE 10000
#define NPRODUCER  100
#define NCONSUMER  100

unsigned long long pcount = 0ULL;
unsigned long long ccount = 0ULL;

typedef struct {
    int stop;
    int buffer[BUFFERSIZE];
    int pindex;
    int cindex;
    tasem_t cmutex;
    tasem_t pmutex;
    tasem_t emptyBuffer;
    tasem_t fullBuffer;
} shared_data_t;


void killerthr(void *vptr)
{
    shared_data_t *data = (shared_data_t*)vptr;
    time_t finish = time(NULL) + 5;
    while (time(NULL) < finish) ta_yield();
    data->stop = TRUE;
}

void producer(void *vptr)
{
    shared_data_t *data = (shared_data_t*)vptr;

    while (!data->stop)
    {
	ta_yield();
	ta_sem_wait(&(data->emptyBuffer));

        // produce
        ta_sem_wait(&(data->pmutex));

        ta_yield();
        printf("producer filling slot %d\n", data->pindex);
        data->buffer[data->pindex] = pcount++;
        data->pindex = (data->pindex + 1) % BUFFERSIZE;

        ta_sem_signal(&(data->pmutex));
	ta_sem_signal(&(data->fullBuffer));
    }
}

void consumer(void *vptr)
{
    shared_data_t *data = (shared_data_t*)vptr;

    while (!data->stop)
    {
	ta_yield();
	ta_sem_wait(&(data->fullBuffer));

        // consume
        ta_sem_wait(&(data->cmutex));

        printf("consumer eating slot %d\n", data->cindex);
        ta_yield();

	assert(data->buffer[data->cindex] == ccount);
        ccount++;
        data->cindex = (data->cindex + 1) % BUFFERSIZE;

        ta_sem_signal(&(data->cmutex));
	ta_sem_signal(&(data->emptyBuffer));
    }
}


int main(int argc, char **argv)
{
    ta_libinit();

    int i;
    shared_data_t shared_data;
    srandom(time(NULL));

    memset(&shared_data, 0, sizeof(shared_data_t));
    for (i = 0 ; i < BUFFERSIZE; i++)
    {
        shared_data.buffer[i] = 0xFEEDFEED;
    }


    ta_sem_init(&shared_data.emptyBuffer, BUFFERSIZE);
    ta_sem_init(&shared_data.fullBuffer, 0);
    ta_sem_init(&shared_data.cmutex, 1);
    ta_sem_init(&shared_data.pmutex, 1);

    ta_create(killerthr, (void *)&shared_data);

    for ( i = 0; i < NPRODUCER; i++)
    {
	ta_create(producer, (void *)&shared_data);
    }

    for ( i = 0; i < NCONSUMER; i++)
    {
	ta_create(consumer, (void *)&shared_data);
    }

    int rv = ta_waitall();
    assert(rv == 0);

    return 0;
}

