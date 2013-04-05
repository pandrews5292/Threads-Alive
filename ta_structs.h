//ta_structs.h

#ifndef TA_STRUCTS_H
#define TA_STRUCTS_H

#define _XOPEN_SOURCE 600

#include <ucontext.h>

typedef struct control_block {
  ucontext_t* ctx;
  int status;
} tcb;

typedef struct queue_node {
    tcb* thread;
    struct queue_node* next;
    struct queue_node* prev;
}queue_node;

typedef struct queue{
    struct queue_node* head;
    struct queue_node* tail;
    int length;
}queue;

typedef struct {
  int value;
  struct queue* w_queue;
}tasem_t;

typedef struct {
  tasem_t* sem; 
}talock_t;

struct sem_node {
  tasem_t* sem;
  struct sem_node* next;
  struct sem_node* prev;
};

typedef struct sema_queue{
  struct sem_node* head;
  struct sem_node* tail;
  int length;
}sema_queue;

#endif
