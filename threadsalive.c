// threadsalive.c

#include "threadsalive.h"

queue* t_queue;
static ucontext_t temp_context;

void ta_libinit(void) {
  t_queue = create_queue();
}

void ta_create(void (*func)(void), void* arg) {
  //printf("%d\n", get_length(t_queue));
  ucontext_t new_thread;
  getcontext(&new_thread);
  void* new_stack = malloc(524288);
  getcontext(&temp_context);

  new_thread.uc_stack.ss_sp = new_stack;
  new_thread.uc_stack.ss_size = sizeof(new_stack);
  new_thread.uc_link = &temp_context;
  makecontext(&new_thread, func, 1, arg);
  push(t_queue, new_thread);
  //printf("%d\n", get_length(t_queue));
}

void ta_yield(void) {
  ucontext_t new_thread = pop(t_queue);
  swapcontext(&temp_context, &new_thread);
  push(t_queue, temp_context);
}

  
