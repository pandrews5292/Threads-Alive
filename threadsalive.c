// threadsalive.c

#include "threadsalive.h"

static void ta_libinit(void) {
  queue* t_queue = create_queue();
}

static void ta_create(void (*func)(void*), void* arg) {
  ucontext_t* new_thread = malloc(sizeof(ucontext_t));
  void* new_stack = malloc(524288);
  new_thread->uc_stack.ss_sp = new_stack;
  new_thread->uc_stack.ss_size = sizeof(new_stack);
  new_thread->uc_link = new_thread;//POINTER_TO_QUEUE_HEAD
  makecontext(new_thread, &func, 1, &arg);
  swapcontext(&filler, new_thread);
}

static void ta_yield(void){
    int get_ctx_ret, set_ctx_ret;
    ucontext_t* cur_thread;
    get_ctx_ret = getcontext(cur_thread);
    push(t_queue, cur_thread);
    ucontext_t* new_thread = pop(t_queue);
}

  
