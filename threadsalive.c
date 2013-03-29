// threadsalive.c

#include "threadsalive.h"
#define STACK_SIZE 524288

static queue* t_queue;
static ucontext_t main_ctx;
static int wait_all_flag = 0;

void ta_libinit(void) {
  t_queue = create_queue();
  getcontext(&main_ctx);
}

void ta_create(void (*func)(void*), void* arg) {
  //printf("%d\n", get_length(t_queue));

  ucontext_t new_thread;
  getcontext(&new_thread);
 
  new_thread.uc_stack.ss_sp = malloc(STACK_SIZE);
  new_thread.uc_stack.ss_size = STACK_SIZE;
  new_thread.uc_link = &main_ctx;
  
  if (arg)
    makecontext(&new_thread, func, 1, *((int*)arg));
  else
    makecontext(&new_thread, func, 0);
  push(t_queue, new_thread);
  //printf("%d\n", get_length(t_queue));
}

void ta_yield(void) {

  ucontext_t cur_ctx;
  if (!get_length(t_queue)) {
    printf("QUEUE EMPTY. Switching to main\n");
    getcontext(&cur_ctx);
    push(t_queue, cur_ctx);
    setcontext(&main_ctx);
  }
  
  else {
    ucontext_t new_thread = pop(t_queue);
    if (main_ctx.uc_stack.ss_sp != cur_ctx.uc_stack.ss_sp)
      push(t_queue, cur_ctx);
    swapcontext(&cur_ctx, &new_thread);
  }
}

int ta_waitall(void) {
  wait_all_flag = 1;
  
  while(get_length(t_queue)) {
    ucontext_t next_ctx = pop(t_queue);
    swapcontext(&main_ctx, &next_ctx);
  }
  wait_all_flag = 0;

  return 0;
}
