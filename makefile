OPTS = -Wall -Wno-unused-function -g -fPIC -Wno-deprecated-declarations

all: test_threads

queue.o: queue.c queue.h
	gcc -c queue.c ${OPTS}

ta_mutex.o: ta_mutex.c ta_mutex.h
	gcc -c ta_mutex.c ${OPTS}

threadsalive.o: threadsalive.c threadsalive.h
	gcc -c threadsalive.c ${OPTS}

libthreadsalive.so: threadsalive.o queue.o ta_mutex.o
	gcc -o libthreadsalive.so threadsalive.o queue.o ta_mutex.o -Wall -g -shared 

test_threads: test_threads.c libthreadsalive.so
	gcc -o test_threads test_threads.c -L. -lthreadsalive -g

clean:
	rm -f test_threads
	rm -f *~
	rm -f \#*
	rm -f *.o
	rm -f *.so
	rm -rf *.dSYM

