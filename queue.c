#include "queue.h"

struct queue* create_queue(){
    //create scheduling queue
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->length = 0;
    return q;
}

struct queue_node* create_queue_node(ucontext_t thread){
    //create a queue_node
    struct queue_node* n = (struct queue_node*)malloc(sizeof(struct queue_node));
    n->thread = thread;
    return n;
}

int get_length(struct queue* q){
    //get length of queue
    return q->length;
}

void push(struct queue* q, ucontext_t thread){
    //push a thread onto the queue
    struct queue_node* n = create_queue_node(thread);
    int i=0;

    if (get_length(q) == 0){
	q->head = n;
	q->length++;
    }
    else{
	struct queue_node* cur = q->head;
	for (;i<get_length(q)-1;i++){
	    cur = cur->next;
	}
	cur->next = n;
	q->length++;
    }
}

ucontext_t pop(struct queue* q){
    //pop a message of the queue

    if (q->length > 1){
	ucontext_t thread = q->head->thread;
	struct queue_node* new_head = q->head->next;
	free(q->head);
	q->head = new_head;
	q->length--;
	return thread;
    }
    else if (q->length == 1){
	ucontext_t thread = q->head->thread;
	//free(q->head);
	q->length--;
	return thread;
    }
    else {
	printf("Error: Queue length 0\n");
	exit(1);
    }
}


void show(struct queue* q){
    //print out the queue for debug reasons
}

void destroy_queue(struct queue* q){
    //free malloced nodes and free queue 
    struct queue_node* cur = q->head;
    int i = 0;
    for (;i<get_length(q);i++){
	struct queue_node* next = cur->next;
	free(cur);
	cur = next;
    }
    free(q);
}
