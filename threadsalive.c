// threadsalive.c

#define THREAD_MAX = 100

static ucontext_t threads[THREAD_MAX];

static void ta_create(void (*func)(void*), void* arg) {
  
