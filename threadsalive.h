#include <stdlib.h>
#include <ucontext.h>

static void ta_yield(void);

static void ta_create(void (*func)(void*), void* arg) {


