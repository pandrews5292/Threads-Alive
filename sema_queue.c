//sema_queue.c

#include "sema_queue.h"

sema_queue* sema_create_queue(){
    //create scheduling queue
    sema_queue* q = (sema_queue*)malloc(sizeof(sema_queue));
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;
    return q;
}

struct sem_node* sema_create_queue_node(tasem_t* sem){
    //create a queue_node
    struct sem_node* n = (struct sem_node*)malloc(sizeof(struct sem_node));
    n->sem = sem;
    return n;
}

int sema_len(sema_queue* q){
    //get length of queue
    return q->length;
}

void sema_push(sema_queue* q, tasem_t* sem){
    //push a sem onto the queue
    struct sem_node* n = sema_create_queue_node(sem);

    if (sema_len(q) == 0){
	q->head = n;
	q->tail = n;
	n->next = NULL;
	n->prev = NULL;
	q->length++;
    }
    else{
	q->tail->prev = n;
	n->next = q->tail;
	q->tail = n;
	q->tail->prev = NULL;
	q->length++;
    }
}

tasem_t* sema_pop(sema_queue* q) {
    //pop a context off the queue

    if (q->length > 1){
        tasem_t* sem = q->head->sem;
	struct sem_node* head = q->head;
        q->head = q->head->prev;

	free(head);
	q->head->next = NULL;
	q->length--;
	return sem;
    }
    else if (q->length == 1){
	tasem_t* sem = q->head->sem;
	q->head = NULL;
	q->tail = NULL;
	free(q->head);
	q->length--;
	return sem;
    }
    else {
	printf("Error: Queue length 0\n");
	exit(1);
    }
}

tasem_t* sema_get_head(sema_queue* q){
    return q->head->sem;

}

void sema_show(sema_queue* q){
    //print out the queue for debug reasons
    /*  Irrelevent for printing out contexts //
    int i = 0;
    struct sem_node* cur = q->head;
    for(;i<get_length(q)-1;i++){
	printf("%d -> ", cur->sem);
	cur = cur->prev;
    }
    printf("%d\n", cur->sem);
    */

}

void sema_destroy_queue(sema_queue* q){
  //free malloc'ed nodes and free queue 
  struct sem_node* cur = q->head;
  int i = 0;
  for (;i<sema_len(q);i++){
    struct sem_node* prev = cur->prev;
    free(cur);
    cur = prev;
  }
  free(q);
}
