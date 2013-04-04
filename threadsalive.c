// threadsalive.c

#include "threadsalive.h"
#define STACK_SIZE 524288

static ucontext_t* main_ctx;

tcb* tcb_init() {
    tcb* new_tcb = (tcb*)malloc(sizeof(tcb));
    new_tcb->ctx = (ucontext_t*)malloc(sizeof(ucontext_t));
    getcontext(new_tcb->ctx);
    new_tcb->status = 1;
    
    return new_tcb;
}

void ta_libinit(void) {
    t_queue = create_queue();
    main_ctx = (ucontext_t*)malloc(sizeof(ucontext_t));
}

void ta_create(void (*func)(void), void* arg) {
    
    tcb* new_thread = tcb_init();
    
    new_thread->ctx->uc_stack.ss_sp = malloc(STACK_SIZE);
    new_thread->ctx->uc_stack.ss_size = STACK_SIZE;
    new_thread->ctx->uc_link = main_ctx;
    
    if (arg)
	makecontext(new_thread->ctx, func, 1, *((int*)arg));
    else
	makecontext(new_thread->ctx, func, 0);
    
    push(t_queue, new_thread);
    
}

void ta_yield(void) {
    
    if (len(t_queue)) {
	tcb* next_thread = pop(t_queue);
	ucontext_t* next_ctx = next_thread->ctx;
	next_ctx->uc_link = main_ctx;
	free(next_thread);
	
	tcb* cur_thread = tcb_init();   
	
	push(t_queue, cur_thread);
	swapcontext(cur_thread->ctx, next_ctx); 
	
    }
}

int ta_waitall(void) {
    
    if (!len(t_queue))
	return -1;
    
    while(len(t_queue)) {
	
	tcb* next_thread = pop(t_queue);
	ucontext_t* next_ctx =  next_thread->ctx;
	free(next_thread);
	
	next_ctx->uc_link = main_ctx;
	swapcontext(main_ctx, next_ctx);
	printf("LEN: %d\n", len(t_queue));
    }
    
    return 0;
}

void ta_sem_init(tasem_t* sema, int value) {
  sema->value = value;
  sema->w_queue = create_queue();
}

void ta_sem_destroy(tasem_t* sema) {
  	destroy_queue(sema->w_queue);
  	free(sema);
}

void ta_sem_signal(tasem_t* sema) {
    sema->value++;
    if (sema->value <= 0 && len(sema->w_queue)) {
	tcb* tcb = pop(sema->w_queue);
	push(t_queue, tcb);
    }
}

void ta_sem_wait(tasem_t* sema){
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

void ta_lock_init(talock_t* lock){
  lock->sem = (tasem_t*)malloc(sizeof(tasem_t));
    ta_sem_init(lock->sem, 1);
}

void ta_lock_destroy(talock_t* lock){
    ta_sem_destroy(lock->sem);
    free(lock);
}

void ta_lock(talock_t* lock){
    ta_sem_wait(lock->sem);
}

void ta_unlock(talock_t* lock){
    ta_sem_signal(lock->sem);
}

