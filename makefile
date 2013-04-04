TESTS = 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
OPTS = -Wall -Wno-unused-function -g -fPIC -Wno-deprecated-declarations

testall: libthreadsalive.so
	$(foreach var, $(TESTS), gcc -o test$(var) test$(var).c -L. -lthreadsalive -g;)

queue.o: queue.c queue.h ta_structs.h
	gcc -c queue.c ${OPTS}

sema_queue.o: sema_queue.c sema_queue.h ta_structs.h
	gcc -c sema_queue.c ${OPTS}

threadsalive.o: threadsalive.c threadsalive.h ta_structs.h
	gcc -c threadsalive.c ${OPTS}

libthreadsalive.so: threadsalive.o sema_queue.o queue.o
	gcc -o libthreadsalive.so threadsalive.o sema_queue.o queue.o -Wall -g -shared 

test_threads: test_threads.c libthreadsalive.so
	gcc -o test_threads test_threads.c -L. -lthreadsalive -g

test01: test01.c libthreadsalive.so
	gcc -o test01 test01.c -L. -lthreadsalive -g

clean:
	rm -f test_threads
	rm -f *~
	rm -f \#*
	rm -f *.o
	rm -f *.so
	rm -rf *.dSYM
	rm -f test01
	rm -f test02
	rm -f test03
	rm -f test04
	rm -f test05
	rm -f test06
	rm -f test07
	rm -f test08
	rm -f test09
	rm -f test10
	rm -f test11
	rm -f test12
	rm -f test13
	rm -f test14
	rm -f test15

