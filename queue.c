//queue.c

#include "queue.h"

queue* create_queue(){
    //create scheduling queue
    queue* q = (queue*)malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;
    return q;
}

queue_node* create_queue_node(tcb* thread){
    //create a queue_node
    queue_node* n = (queue_node*)malloc(sizeof(queue_node));
    n->thread = thread;
    return n;
}

int len(queue* q){
    //get length of queue
    return q->length;
}

void push(queue* q, tcb* thread){
    //push a thread onto the queue
    queue_node* n = create_queue_node(thread);

    if (len(q) == 0){
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

tcb* pop(queue* q) {
    //pop a context off the queue

    if (q->length > 1){
        tcb* thread = q->head->thread;
	queue_node* head = q->head;
        q->head = q->head->prev;

	free(head);
	q->head->next = NULL;
	q->length--;
	return thread;
    }
    else if (q->length == 1){
	tcb* thread = q->head->thread;
	q->head = NULL;
	q->tail = NULL;
	free(q->head);
	q->length--;
	return thread;
    }
    else {
	printf("Error: Queue length 0\n");
	exit(1);
    }
}

tcb* get_head(queue* q){
    return q->head->thread;

}

void show(queue* q){
    //print out the queue for debug reasons
    /*  Irrelevent for printing out contexts //
    int i = 0;
    queue_node* cur = q->head;
    for(;i<get_length(q)-1;i++){
	printf("%d -> ", cur->thread);
	cur = cur->prev;
    }
    printf("%d\n", cur->thread);
    */

}

void destroy_queue(queue* q){
    //free malloc'ed nodes and free queue 
    queue_node* cur = q->head;
    int i = 0;
    for (;i<len(q);i++){
	queue_node* prev = cur->prev;
	free(cur->thread->ctx);
	free(cur->thread);
	free(cur);
	cur = prev;
    }
    free(q);
}
