#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "../header/thread_function.h"
#include "../header/time_stamp.h"
#include "../header/thread_args.h"
#include "../header/millisecond_sleep.h"

void* periodic_task(void* thread_args)
{   
    struct periodic_task_information* pti = (struct periodic_task_information*) thread_args; 

    //<Local Variables>
    //int task_num; //Debugging?
    int* loop_iter; //Needed
    pthread_mutex_t* thread_mutex; //Pass actual mutex instead...
    int period; //Needed
    volatile bool* activate;
    volatile bool* terminate;
    long long start_time;
    long long stop_time;

    //<Initialization>
    //task_num = pti->task_num;
    loop_iter = pti->loop_iter;
    thread_mutex = pti->thread_mutex;
    period = pti->period;
    activate = pti->activate;
    terminate = pti->terminate;
    
    printf("main_thread: %d\n", gettid());

    //<Wait for Activation>
    while(!(*activate)){millisleep(1000);} //Busy wait...
    
    while(!(*terminate)) //Check if termination occurred...
    {
        start_time = milliseconds_time(); //Get time stamp at start of thread
        
        computation(loop_iter[0]); //Computation 1
        pthread_mutex_lock(thread_mutex); //Lock mutex
        computation(loop_iter[1]); //Computation 2 
        pthread_mutex_unlock(thread_mutex); //Unlock mutex
        computation(loop_iter[2]); //Computation 3
       
        stop_time = milliseconds_time(); //Get time stamp at end of thread
        
        if((stop_time - start_time) < period) //Check if there is any leftover time at that period
        {
            millisleep(period - (stop_time - start_time)); //Sleep for the rest of period if there is
        } //Otherwise continue
    }
    //end while loop
    return NULL;
}

void* aperiodic_task(void* thread_args)
{
    
    struct aperiodic_task_information* ati = (struct aperiodic_task_information*) thread_args;

    //<Local Variables>
    int task_num; //Debugging?
    volatile bool* event_occured; //Needed
    int* loop_iter; //Needed
    pthread_mutex_t* thread_mutex; //Pass actual mutex instead...
    volatile bool* activate;
    volatile bool* terminate;

    //<Initialization>
    task_num =  ati->task_num;
    event_occured = ati->event_occurred;
    loop_iter = ati->loop_iter;
    thread_mutex = ati->thread_mutex;
    activate = ati->activate;
    terminate = ati->terminate;

    printf("main_thread %d\n", gettid());

    //<Wait for Activation>
    while(!(*activate)){millisleep(1000);} //Busy wait...
    while(!(*event_occured) && !(*terminate)) {millisleep(1000);  printf("Task %d: Checking event...\n", task_num);}
    while(!(*terminate)) //Busy wait... check if terminated?
    {
        printf("Starting computations in Task %d...\n", task_num);
        computation(loop_iter[0]);
        pthread_mutex_lock(thread_mutex); //Lock mutex
        computation(loop_iter[1]);
        pthread_mutex_unlock(thread_mutex); //Unlock mutex
        computation(loop_iter[2]);
        printf("Finished computations in Task %d...\n", task_num);

        (*event_occured) = false; //Reset event flag...

        while(!(*event_occured) && !(*terminate)) {millisleep(1000);  printf("Task %d: Checking event...\n", task_num);}
    }
    
    return NULL;
}

void computation(uint64_t val)
{
    volatile uint64_t x = val;
    while(x > 0)
    {
        x--;
    }
}