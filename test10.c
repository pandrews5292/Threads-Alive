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

#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef struct {
    int stop;

    int atTable;
    tasem_t atTableSem;

    tasem_t mutex;
    int state[NPHILOSOPHERS];
    tasem_t mayEat[NPHILOSOPHERS];
} shared_data_t;

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

void testSafetyAndLiveness(int i, shared_data_t *data)
{
    if (data->state[i] == HUNGRY &&
	(data->state[(i+1)%NPHILOSOPHERS] != EATING &&
	 data->state[(i-1)%NPHILOSOPHERS] != EATING)) 
    {
	data->state[i] = EATING;
	ta_sem_signal(&data->mayEat[i]);
    }
}

void take_chopsticks(int i, shared_data_t *data)
{
    fprintf(stderr, "Philosopher %d waiting to chopsticks %d and %d\n", i, i, (i+1)%NPHILOSOPHERS);

    ta_sem_wait(&data->mutex);
    data->state[i] = HUNGRY;
    testSafetyAndLiveness(i, data);
    ta_sem_signal(&data->mutex);
    ta_sem_wait(&data->mayEat[i]);
}
    

void release_chopsticks(int i, shared_data_t *data)
{
    fprintf(stderr, "Philosopher %d is releasing chopsticks %d and %d\n", i, i, (i+1)%NPHILOSOPHERS);
    
    ta_sem_wait(&data->mutex);
    data->state[i] = THINKING;
    testSafetyAndLiveness((i+1)%NPHILOSOPHERS, data);
    testSafetyAndLiveness((i-1)%NPHILOSOPHERS, data);
    ta_sem_signal(&data->mutex);
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


int main(int argc, char **argv)
{
    ta_libinit();
    shared_data_t shared_data;
    int i;
   
    srandom(time(NULL));
    memset(&shared_data, 0, sizeof(shared_data_t));

    ta_sem_init(&shared_data.atTableSem, 1);
    ta_sem_init(&shared_data.mutex, 1);
    for (i = 0 ; i < NPHILOSOPHERS; i++)
    {
        ta_sem_init(&shared_data.mayEat[i], 0); 
    }

    ta_create(killerthr, (void*)&shared_data);
    for (i = 0; i < NPHILOSOPHERS; i++)
    {
        ta_create(philosophize, (void *)&shared_data);
    }

    int rv = ta_waitall();
    assert(rv == 0);

    return 0;
}

