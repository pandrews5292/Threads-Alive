#include "threadsalive.h"
#include <stdio.h>

void add(){
    
    int i = 0, n = 0;
    for (;i<10;i++){
	printf("%d\n",n+i);
    }

}

int main(){
    ta_libinit();

    ta_create(&add, NULL);

    int i = 0, n = 10;
    for (;i<10;i++){
      if (n+i==15)
	ta_yield();
      printf("%d\n",n+i);
    }
       
    return 0;
}
