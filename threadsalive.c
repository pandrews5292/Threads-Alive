// threadsalive.c

#include "threadsalive.h"
#define STACK_SIZE = 524288

static queue* t_queue;
static ucontext_t main_ctx;
static int first_swap = 1;
static int wait_all_flag = 0;

void ta_libinit(void) {
  t_queue = create_queue();
  getcontext(&main_ctx);
  getcontext(&cur_ctx);
}

void ta_create(void (*func)(void*), void* arg) {
  //printf("%d\n", get_length(t_queue));
  if (wait_all_flag) {
    t_queue = create_queue();
    wait_all_flag--;
  }

  ucontext_t new_thread;
  getcontext(&new_thread);

  new_thread.uc_stack.ss_sp = malloc(STACK_SIZE);
  new_thread.uc_stack.ss_size = STACK_SIZE;
  new_thread.uc_link = &main_ctx;

  if (arg)
    makecontext(&new_thread, func(), 1, *((int*)arg));
  else
    makecontext(&new_thread, func(), 0);
  push(t_queue, new_thread);
  //printf("%d\n", get_length(t_queue));
}

void ta_yield(void) {
  if (wait_all_flag) {
    t_queue = create_queue();
    wait_all_flag--;
  }

  else {
    ucontext_t cur_ctx;
    if (!get_length(t_queue)) {
      printf("QUEUE EMPTY. Switching to main\n");
      setcontext(&main_ctx);
    }
    
    else {
      ucontext_t new_thread = pop(t_queue);
      swapcontext(&cur_context, &new_thread);
      if (main_ctx.uc_stack.ss_sp != cur_ctx.uc_stack.ss_sp)
	push(t_queue, cur_context);
    }
  }
}

int ta_waitall(void) {
  wait_all_flag = 1;
  
  queue_node* cur = getHead(t_queue);
  while(cur) {
    if(getNext(cur)) {
      fetch_ctx(cur).ctx.uc_link =
	}
    else 
      fetch_ctx(cur).ctx.uc_link = &main_ctx;
    cur = getNoext(cur);
  }
}
