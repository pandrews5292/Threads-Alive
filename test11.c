#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <assert.h>

#include "threadsalive.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define BUFFERSIZE 10

typedef struct {
    int stop;
    int buffer[BUFFERSIZE];

    tasem_t emptyBuffer;
    tasem_t fullBuffer;
} shared_data_t;


void killerthr(void *vptr)
{
    shared_data_t *data = (shared_data_t*)vptr;
    time_t finish = time(NULL) + 5;
    while (time(NULL) < finish)
    {
        ta_yield();
    }
    data->stop = TRUE;
}

void producer(void *vptr)
{
    shared_data_t *data = (shared_data_t*)vptr;
    int i = 0;
    unsigned long long count = 0ULL;

    while (!data->stop)
    {
	ta_yield();
        ta_sem_wait(&(data->emptyBuffer));

        // produce
	ta_yield();
        printf("producer filled slot %d\n", i);

	data->buffer[i] = count++;

	i = (i+1)%BUFFERSIZE;
	ta_sem_signal(&(data->fullBuffer));
    }
}

void consumer(void *vptr)
{
    shared_data_t *data = (shared_data_t*)vptr;
    int j = 0;
    unsigned long long expect_count = 0ULL;

    while (!data->stop)
    {
	ta_yield();
	ta_sem_wait(&(data->fullBuffer));

        // consume
        ta_yield();
        printf("consumer read slot %d\n", j);
        assert(data->buffer[j] == expect_count);
        expect_count++;

	j = (j+1)%BUFFERSIZE;
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

    ta_create(killerthr, (void *)&shared_data);
    ta_create(producer, (void *)&shared_data);
    ta_create(consumer, (void *)&shared_data);

    int rv = ta_waitall();
    assert(rv == 0);

    return 0;
}

