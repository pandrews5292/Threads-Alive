//ta_structs.h

#ifndef TA_STRUCTS_H
#define TA_STRUCTS_H

#define _XOPEN_SOURCE 600

#include <ucontext.h>

/* A structure representing a thread control block.
   Contains a pointer to the thread's context, and
   an integer that can indicate the status of the
   thread (running, waiting). */
typedef struct control_block {
  ucontext_t* ctx;
  int status;
} tcb;


/* Node for a queue containing TCBs. */
typedef struct queue_node {
    tcb* thread;
    struct queue_node* next;
    struct queue_node* prev;
}queue_node;

/* A queue for scheduling threads. */
typedef struct queue{
    struct queue_node* head;
    struct queue_node* tail;
    int length;
}queue;

/* A semaphone structure. Contains a value and a
   queue of threads waiting on the semaphore. */
typedef struct {
  int value;
  struct queue* w_queue;
}tasem_t;

/* A lock structure. Implemeted entirely using
   semaphores. */
typedef struct {
  tasem_t* sem; 
}talock_t;

/* A node for a queue of semaphores. */
struct sem_node {
  tasem_t* sem;
  struct sem_node* next;
  struct sem_node* prev;
};

/* A queue of semaphores. Used for easy access
   to all created semaphores. */
typedef struct sema_queue{
  struct sem_node* head;
  struct sem_node* tail;
  int length;
}sema_queue;

#endif
