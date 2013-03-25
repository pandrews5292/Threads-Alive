// threadsalive.c

#include "threadsalive.h"
#include "queue.h"

static void ta_create(void (*func)(void*), void* arg) {
  ucontext_t* new_thread = malloc(sizeof(ucontext_t));
  void* new_stack = malloc(524288);
  new_thread->uc_stack.ss_sp = new_stack;
  new_thread->uc_stack.ss_size = sizeof(new_stack);
  new_thread->uc_link = new_thread;//POINTER_TO_QUEUE_HEAD
  makecontext(new_thread, func, 1, &arg);
  swapcontext(&filler, new_thread);
}

static void ta_yield(void){
    int get_ctx_ret, set_ctx_ret;
    ucontext_t* cur_thread = malloc(sizeof(ucontext_t));
    get_ctx_ret = getcontext(cur_thread);
    set_ctx_ret = setcontext(cur_thread->uc_link);
    
    
    

    
}

  
