OPTS = -Wall -Wno-unused-function -g -fPIC

queue.o: queue.c queue.h
	gcc -c queue.c ${OPTS}

threadsalive.o: threadsalive.c threadsalive.h
	gcc -c threadsalive.c ${OPTS}

libthreadsalive.so: threadsalive.o
	gcc -o libthreadsalive.so threadsalive.o queue.o -Wall -g -shared 

clean:
	rm -f *~
	rm -f *.o

