OPTS = -Wall -g -fpic

queue.o: queue.c queue.h
	gcc ${OPTS} -c queue.c

threadsalive.o: threadsalive.c threadsalive.h
	gcc ${OPS} -c threadsalive.c

threadsalive.so: threadsalive.o
	gcc -o threadsalive.so threadsalive.o -Wall -g -shared 