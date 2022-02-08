CC = gcc
CFLAGS = -c -Wall
THREAD_FLAG = -pthread
DEBUG = -g
OUTPUT = -o

debug: main.o thread_function.o keyboard_thread.o time_stamp.o millisecond_sleep.o
	$(CC) $(DEBUG) main.o thread_function.o keyboard_thread.o time_stamp.o millisecond_sleep.o $(THREAD_FLAG) $(OUTPUT) main_thread

main_thread: main.o thread_function.o keyboard_thread.o time_stamp.o millisecond_slepp.o
	$(CC) main.o thread_function.o keyboard_thread.o time_stamp.o millisecond_sleep.o $(THREAD_FLAG) $(OUTPUT) main_thread

main.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(CFLAGS) source/main.c

thread_function.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(CFLAGS) source/thread_function.c

keyboard_thread.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(CFLAGS) source/keyboard_thread.c

time_stamp.o:	
	$(CC) $(DEBUG) $(THREAD_FLAG) $(CFLAGS) source/time_stamp.c

millisecond_sleep.o:
	$(CC) $(DEBUG) $(THREAD_FLAG) $(CFLAGS) source/millisecond_sleep.c

run:
	sudo ./main_thread

clean:
	rm *.o main_thread
