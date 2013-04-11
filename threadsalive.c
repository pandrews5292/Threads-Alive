// threadsalive.c

#include "threadsalive.h"
#define STACK_SIZE 524288

static struct queue* t_queue;
static struct sema_queue* s_queue;
static ucontext_t* main_ctx;
static int sema_flag = 0;

tcb* tcb_init() {
    //initialize a new tcb which calls getcontext() to 
    //hold the current context in a member called ctx.
    tcb* new_tcb = (tcb*)malloc(sizeof(tcb));
    new_tcb->ctx = (ucontext_t*)malloc(sizeof(ucontext_t));
    getcontext(new_tcb->ctx);
    new_tcb->status = 1;
    
    return new_tcb;
}

void ta_libinit(void) {
    //initialize the ta library
    t_queue = create_queue(); //scheduling queue

    s_queue = sema_create_queue();  //queue to hold all initizalized semaphores
    main_ctx = (ucontext_t*)malloc(sizeof(ucontext_t));
}

void ta_create(void (*func)(void*), void* arg) {
    //create a new tcb/context which will run the desired func
    //when swapped to
    
    tcb* new_thread = tcb_init();
    
    new_thread->ctx->uc_stack.ss_sp = malloc(STACK_SIZE);
    new_thread->ctx->uc_stack.ss_size = STACK_SIZE;
    new_thread->ctx->uc_link = main_ctx;
    
    //if (arg)
    makecontext(new_thread->ctx, func, 1, arg);
    //else
    //makecontext(new_thread->ctx, func, 0);
    
    push(t_queue, new_thread);
    
}

void ta_yield(void) {
    //yield to a new context
    
    if (len(t_queue)) {
	tcb* next_thread = pop(t_queue);  //grab a new tcb
	ucontext_t* next_ctx = next_thread->ctx;
	next_ctx->uc_link = main_ctx;
	free(next_thread);
	
	tcb* cur_thread = tcb_init();  //create a new tcb to hold current context
	
	push(t_queue, cur_thread);
	swapcontext(cur_thread->ctx, next_ctx); 
	
    }
}

int ta_waitall(void) {
    // run all contexts created by ta_create
    
    if (!len(t_queue))
	return -1;
    
    while(len(t_queue)) {
	
	tcb* next_thread = pop(t_queue);
	ucontext_t* next_ctx =  next_thread->ctx;
	free(next_thread);
	
	next_ctx->uc_link = main_ctx;
	swapcontext(main_ctx, next_ctx);
    }
    while(sema_len(s_queue)){     //return 1 if some thread was left in sema_queue
        tasem_t* sem = sema_pop(s_queue);
        if (len(sem->w_queue)){
            sema_flag = 1;
            break;
        }
    }
    sema_destroy_queue(s_queue);
    destroy_queue(t_queue);
    
    return sema_flag;
}

void ta_sem_init(tasem_t* sema, int value) {
    //initialize a new semaphore
    sema->value = value;
    sema->w_queue = create_queue();
    sema_push(s_queue, sema);
}

void ta_sem_destroy(tasem_t* sema) {
    //destroy semaphore
    if (len(sema->w_queue)){
        sema_flag = 1;  //set flag to tell user thread left on sema_queue
    }
  	destroy_queue(sema->w_queue);
}

void ta_sem_signal(tasem_t* sema) {
    //semaphore V(), decrement semaphore counter by 1.
    sema->value++;
    if (sema->value <= 0 && len(sema->w_queue)) {
	tcb* tcb = pop(sema->w_queue);
	push(t_queue, tcb);
    }
}

void ta_sem_wait(tasem_t* sema){
    //semaphore P(), increment semaphore counter by 1.
    sema->value--;		
    if (sema->value < 0){
	tcb* to_wait = tcb_init();
	push(sema->w_queue, to_wait);
	
	tcb* next_tcb = pop(t_queue);
	ucontext_t* next_ctx = next_tcb->ctx;
	free(next_tcb);
	swapcontext(to_wait->ctx, next_ctx);
    }	
}

//////////////////////////////

void ta_lock_init(talock_t* lock){
    //initialize lock using a binary semaphore
    lock->sem = (tasem_t*)malloc(sizeof(tasem_t));
    ta_sem_init(lock->sem, 1);
}

void ta_lock_destroy(talock_t* lock){
    //destroy lock
    ta_sem_destroy(lock->sem);
    free(lock->sem);
}

void ta_lock(talock_t* lock){
    //lock the lock
    ta_sem_wait(lock->sem);
}

void ta_unlock(talock_t* lock){
    //unlock the lock
    ta_sem_signal(lock->sem);
}

//////////////////////////////

void ta_cond_init(tacond_t* cond) {
    //initialize a condition variable queue for cond_var
    cond->c_queue = create_queue();
}

void ta_cond_destroy(tacond_t* cond) {
    //destroy cond_var queue
    destroy_queue(cond->c_queue);
}


void ta_wait(tacond_t* cond, talock_t* lock) {
    //allow thread to wait on cond_var
    //release lock right away and wait
    ta_unlock(lock);
    tcb* new_tcb = tcb_init();
    push(cond->c_queue, new_tcb);
    tcb* next_tcb = pop(t_queue);
    ucontext_t* next_ctx = next_tcb->ctx;
  
    free(next_tcb);
    swapcontext(new_tcb->ctx, next_ctx);

}

void ta_signal(tacond_t* cond) {
    //signal a waiter on the cond_var
    if(len(cond->c_queue)) {
	tcb* next_thread = pop(cond->c_queue);
	push(t_queue, next_thread);
    }
}
