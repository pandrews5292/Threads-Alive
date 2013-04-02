#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

typedef struct control_block {
  ucontext_t* ctx;
  int status;
} tcb;


struct queue_node {
    tcb* thread;
    struct queue_node* next;
    struct queue_node* prev;
};

typedef struct queue {
    struct queue_node* head;
    struct queue_node* tail;
    int length;
}queue;

void show(struct queue* q);

tcb* pop(struct queue* q);

void push(struct queue* q, tcb*);

int len(struct queue* q);

struct queue* create_queue();

struct queue_node* create_queue_node(tcb*);

tcb* get_head(struct queue* q);

void destroy_queue(struct queue* q);




