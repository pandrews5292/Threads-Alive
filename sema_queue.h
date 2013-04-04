//sema_queue.h
#ifndef SEMA_QUEUE_H
#define SEMA_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "ta_structs.h"

void sema_show(sema_queue*);

tasem_t* sema_pop(sema_queue*);

void sema_push(sema_queue*, tasem_t*);

int sema_len(sema_queue*);

sema_queue* sema_create_queue();

struct sem_node* sema_create_queue_node(tasem_t*);

tasem_t* sema_get_head(sema_queue*);

void sema_destroy_queue(sema_queue*);

#endif

