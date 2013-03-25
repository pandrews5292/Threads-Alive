#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include "queue.h"

static void ta_libinit();
static void ta_create(void *(void*), void*);
static void ta_yield(void);
static int ta_waitall();


