#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct queue_node {
    ucontext_t thread;
    struct queue_node* next;
    struct queue_node* prev;
};

typedef struct queue {
    struct queue_node* head;
    struct queue_node* tail;
    int length;
}queue;

void show(struct queue* q);

ucontext_t pop(struct queue* q);

void push(struct queue* q, ucontext_t thread);

int get_length(struct queue* q);

struct queue* create_queue();

struct queue_node* create_queue_node(ucontext_t thread);

ucontext_t get_head(struct queue* q);

void destroy_queue(struct queue* q);




