// threadsalive.c

#include "threadsalive.h"
#define STACK_SIZE 524288

static queue* t_queue;
static ucontext_t* main_ctx;

static tcb* tcb_init(void) {
  tcb* new_tcb = (tcb*)malloc(sizeof(tcb));
  new_tcb->ctx = (ucontext*)malloc(sizeof(ucontext_t));
  getcontext(new_tcb_ctx);
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

