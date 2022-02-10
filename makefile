CC = gcc
CFLAGS = -c 
WARN = -Wall
THREAD_FLAG = -pthread
DEBUG = -g
OUTPUT = -o

assignment1: main.o thread_function.o keyboard_thread.o millisecond_sleep.o
	$(CC) $(DEBUG) $(THREAD_FLAG) $(WARN) main.o thread_function.o keyboard_thread.o millisecond_sleep.o $(OUTPUT) assignment1

main.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(WARN) $(CFLAGS) source/main.c

thread_function.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(WARN) $(CFLAGS) source/thread_function.c

keyboard_thread.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(WARN) $(CFLAGS) source/keyboard_thread.c

millisecond_sleep.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(WARN) $(CFLAGS) source/millisecond_sleep.c

run:
	sudo trace-cmd record -e sched_switch taskset -c 3 ./assignment1

clean:
	rm *.o assignment1
