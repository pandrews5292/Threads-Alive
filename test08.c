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

#define NPHILOSOPHERS 5


typedef struct {
    int stop;

    int atTable;
    tasem_t atTableSem;
    tasem_t chopstickSem[NPHILOSOPHERS];
} shared_data_t;


void killerthr(void *v)
{
    shared_data_t *d = (shared_data_t*)v;
    time_t finish = time(NULL) + 5;

    while (time(NULL) < finish)
    {
        ta_yield(); 
    }
    d->stop = TRUE;
}


void think(int i)
{
    fprintf(stderr, "Philosopher %d is thinking\n", i);
    ta_yield();
}

void eat(int i)
{
    fprintf(stderr, "Philosopher %d is eating\n", i);
    ta_yield();
}

void take_chopsticks(int i, shared_data_t *data)
{
    fprintf(stderr, "Philosopher %d is taking chopsticks %d and %d\n", i, i, (i+1) % NPHILOSOPHERS);
    ta_sem_wait(&data->chopstickSem[i]);
    ta_sem_wait(&data->chopstickSem[(i+1)%NPHILOSOPHERS]);
}

void release_chopsticks(int i, shared_data_t *data)
{
    fprintf(stderr, "Philosopher %d is releasing chopsticks %d and %d\n", i, i, (i+1)%NPHILOSOPHERS);
    ta_sem_signal(&data->chopstickSem[i]);
    ta_sem_signal(&data->chopstickSem[(i+1)%NPHILOSOPHERS]);
}


void philosophize(void *vptr)
{
    shared_data_t *data = (shared_data_t*)vptr;
    int philosopher_index = 0;

    ta_sem_wait(&data->atTableSem);
    philosopher_index = data->atTable++; 
    ta_sem_signal(&data->atTableSem);
    fprintf(stderr, "Philosopher %d has been seated\n", philosopher_index);

    while (!data->stop)
    {
        think(philosopher_index);
        take_chopsticks(philosopher_index, data);
        eat(philosopher_index);
        release_chopsticks(philosopher_index, data);
    }
}


int main(int argc, char **argv)
{
    ta_libinit();

    shared_data_t shared_data;
    int i;
    srandom(time(NULL));
    memset(&shared_data, 0, sizeof(shared_data_t));

    ta_sem_init(&shared_data.atTableSem, 1); 
    for (i = 0 ; i < NPHILOSOPHERS; i++)
    {
        ta_sem_init(&shared_data.chopstickSem[i], 1); 
    }

    ta_create(killerthr, (void*)&shared_data);
    for (i = 0; i < NPHILOSOPHERS; i++)
    {
        ta_create(philosophize, (void *)&shared_data);
    }

    int rv = ta_waitall();
    assert(rv < 0);

    return 0;
}

