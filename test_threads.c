#include "threadsalive.h"
#include <stdio.h>

void add(){
    
    int i = 0, n = 0;
    for (;i<10;i++){
      if (i > 7) {
	ta_yield();
      }
      printf("%d\n",n+i);
    }

}

void add1(int x) {
  
  int i=0, n=0;
  for (;i<6;i++) {
    if (i > 3)
      ta_yield();
    printf("%d\n",x+i);
  }
}

int main(){
    ta_libinit();

    int x = -6;
    int* x_p = &x;

    ta_create(&add, NULL);
    ta_create(&add1, (void*)x_p);

    ta_waitall();

    return 0;
}
