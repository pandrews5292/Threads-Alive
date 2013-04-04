#ifndef QUEUE_H
#define QUEUE_H

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include "ta_structs.h"

void show(queue* q);

tcb* pop(queue* q);

void push(queue* q, tcb*);

int len(queue* q);

queue* create_queue();

struct queue_node* create_queue_node(tcb*);

tcb* get_head(queue* q);

void destroy_queue(queue* q);

#endif

