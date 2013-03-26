#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

struct queue_node {
    ucontext_t thread;
    struct queue_node* next;
};

typedef struct queue {
    struct queue_node* head;
    int length;
}queue;

void show(struct queue* q);

ucontext_t pop(struct queue* q);

void push(struct queue* q, ucontext_t thread);

int get_length(struct queue* q);

struct queue* create_queue();

struct queue_node* create_queue_node(ucontext_t thread);

void destroy_queue(struct queue* q);




