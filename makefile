OPTS = -Wall -Wno-unused-function -g -fPIC


queue.o: queue.c queue.h
	gcc -c queue.c ${OPTS}

threadsalive.o: threadsalive.c threadsalive.h
	gcc -c threadsalive.c ${OPTS}

libthreadsalive.so: threadsalive.o queue.o
	gcc -o libthreadsalive.so threadsalive.o queue.o -Wall -g -shared 

test0: test_threads.c libthreadsalive.so
	gcc -o test_threads test_threads.c -L. -lthreadsalive

clean:
	rm -f test_threads
	rm -f *~
	rm -f *.o
	rm -f *.so
