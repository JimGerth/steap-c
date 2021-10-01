CC = gcc
CFLAGS = -Wall -Wextra

thread_pool: thread_pool.o task.o queue.o
	$(CC) $(CFLAGS) thread_pool.o task.o queue.o -o thread_pool

thread_pool.o: thread_pool.c task.h queue.h
	$(CC) $(CFLAGS) -c thread_pool.c

task.o: task.c task.h
	$(CC) $(CFLAGS) -c task.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c
