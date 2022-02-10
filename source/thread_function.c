#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "../header/thread_function.h"
#include "../header/thread_args.h"
#include "../header/millisecond_sleep.h"

//Thread function to run a periodic task
void* periodic_task(void* thread_args)
{   
    //Create the stuct that will store the arguments passed to the thread function
    struct periodic_task_information* pti = (struct periodic_task_information*) thread_args; 

    //Create local variables for the thread function
    int* loop_iter; 
    pthread_mutex_t* thread_mutex;
    int period;
    volatile bool* terminate;
    pthread_cond_t* activate;
    pthread_cond_t* stop_sleep;

    //Set the local variables to the thread arguments provided
    loop_iter = pti->loop_iter;
    thread_mutex = pti->thread_mutex;
    period = pti->period;
    terminate = pti->terminate;
    activate = pti->activate_condition;
    stop_sleep = pti->stop_sleep;
    
    //Thread sits here and waits for activation signal from the user
    pthread_cond_wait(activate, thread_mutex);
    pthread_mutex_unlock(thread_mutex);
    
    //Do this computation while we are not in terminate state
    while(!(*terminate))
    {
        //Retrieve the start time of the computation
        struct timespec start_time;
        clock_gettime(CLOCK_REALTIME, &start_time);

        //Begin the computation sequence
        computation(loop_iter[0]); //Computation 1
        pthread_mutex_lock(thread_mutex); //Lock mutex
        computation(loop_iter[1]); //Computation 2 
        pthread_mutex_unlock(thread_mutex); //Unlock mutex
        
        
        computation(loop_iter[2]); //Computation 3
       
        //Retrieve the end time of the computation
        struct timespec stop_time;
        clock_gettime(CLOCK_REALTIME, &stop_time);
        
        //Convert the period time to nanoseconds
        int64_t period_s_ns = period;
        period_s_ns *= 1000000;

        //Take delta of the stop time in seconds and start time in seconds
        //Convert delta seconds to nanoseconds 
        int64_t delta_s_ns = (stop_time.tv_sec - start_time.tv_sec);
        delta_s_ns *= 1000000000;

        //Take the delta of the nanosecond time between stop and start
        int64_t delta_ns_ns = (stop_time.tv_nsec - start_time.tv_nsec);

        //Finally take the intermediate results calculated above
            //and determine if delta_ns is greater than 0
        int64_t delta_ns = period_s_ns - delta_s_ns - delta_ns_ns;
        if(delta_ns > 0 && !(*terminate))
        {
            struct timespec delta;
            struct timespec current_time;

            //We need to take the delta_ns value and place it in a timespec struct
            delta.tv_sec = delta_ns / 1000000000;
            delta.tv_nsec = (delta_ns % 1000000000);

            //Grab current time of the system since pthread_cond_timedwait uses absolute time...
            //We take the delta value and add it to the absolute time to get the timer ready
            clock_gettime(CLOCK_REALTIME, &current_time);
            current_time.tv_nsec = (current_time.tv_nsec + delta.tv_nsec) % 1000000000;
            current_time.tv_sec = current_time.tv_sec + delta.tv_sec + (current_time.tv_nsec / 1000000000);

            //The thread sits and waits either for the corresponding time or for the stop_sleep condition to occur
            pthread_cond_timedwait(stop_sleep, thread_mutex, &current_time);
            pthread_mutex_unlock(thread_mutex); //Lock mutex
        } 
    }
    return NULL;
}

//Thread function for an aperiodic task
void* aperiodic_task(void* thread_args)
{
    //Create the struct that will retrieve the arguments passed to the thread
    struct aperiodic_task_information* ati = (struct aperiodic_task_information*) thread_args;

    //Create local variables to retrieve these argument values
    int* loop_iter;
    pthread_mutex_t* thread_mutex; 
    volatile bool* terminate;
    pthread_cond_t* activate;
    pthread_cond_t* event_occurred;

    //Set the local variables to the values of the passed arguments
    event_occurred = ati->event_occurred;
    loop_iter = ati->loop_iter;
    thread_mutex = ati->thread_mutex;
    terminate = ati->terminate;
    activate = ati->activate_condition;

    //Wait for the activation event to occur before continuing
    pthread_cond_wait(activate, thread_mutex);

    //Wait for an event to happen before we run
    pthread_cond_wait(event_occurred, thread_mutex);
    pthread_mutex_unlock(thread_mutex); 
    
    //Check if we have reached termination state
    while(!(*terminate))
    {
        //Start of computation sequence
        computation(loop_iter[0]);
        pthread_mutex_lock(thread_mutex); //Lock mutex
        computation(loop_iter[1]);
        pthread_mutex_unlock(thread_mutex); //Unlock mutex
        computation(loop_iter[2]);

        //Lock the thread again and wait for an event to occur
        pthread_cond_wait(event_occurred, thread_mutex);
        pthread_mutex_unlock(thread_mutex); //Unlock mutex
    }
    
    return NULL;
}

//Simple computation function
void computation(uint64_t val)
{
    volatile uint64_t x = val;
    while(x > 0)
    {
        x--;
    }
}