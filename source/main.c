#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

#include "../header/thread_args.h"
#include "../header/thread_model.h"
#include "../header/keyboard_thread.h"
#include "../header/thread_function.h"
#include "../header/millisecond_sleep.h"


int main()
{
    //<Local variables>
    const int NUM_EVENTS = 10;
    volatile bool* terminated;
    int num_pti = 0;
    int num_ati = 0;
    int num_thread_id = 0;
    int index = 0;

    //<Local Variables>
    pthread_cond_t* activate_condition;
    pthread_cond_t** event_flags;
    pthread_cond_t* stop_sleep;
    pthread_mutex_t** mutex;
    pthread_attr_t thread_attr;

    //Memory location holders for cleanup later
    struct periodic_task_information* pti_memory_holder[NUM_THREADS];
    struct aperiodic_task_information* ati_memory_holder[NUM_THREADS];
    struct periodic_task_information* pti;
    struct aperiodic_task_information* ati;
    pthread_t thread_ids[NUM_THREADS + 1];

    //Initialize the mutexes that the threads will use later during computation
    mutex = malloc(NUM_MUTEXES * sizeof(pthread_mutex_t*));
    index = 0;
    while(index < NUM_MUTEXES)
    {
        mutex[index] = malloc(sizeof(pthread_mutex_t));
        index++;
    }

    //Initialize condition variable to start the thread execution
    activate_condition = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(activate_condition, NULL);
    
    //Initialize condition variable that will be used to wake up sleep threads
    stop_sleep = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(stop_sleep, NULL);

    //Initialize variable used to terminate all threads when finished
    terminated = malloc(sizeof(bool));
    (*terminated) = false;

    //Initialize event flags used by threads to determine an event has occurred
    event_flags = malloc(NUM_EVENTS * sizeof(pthread_cond_t*));

    //Each event flag is given dynamic memory to a condition variable
    event_flags[0] = malloc(sizeof(pthread_cond_t));
    event_flags[1] = malloc(sizeof(pthread_cond_t));
    event_flags[2] = malloc(sizeof(pthread_cond_t));
    event_flags[3] = malloc(sizeof(pthread_cond_t));
    event_flags[4] = malloc(sizeof(pthread_cond_t));
    event_flags[5] = malloc(sizeof(pthread_cond_t));
    event_flags[6] = malloc(sizeof(pthread_cond_t));
    event_flags[7] = malloc(sizeof(pthread_cond_t));
    event_flags[8] = malloc(sizeof(pthread_cond_t));
    event_flags[9] = malloc(sizeof(pthread_cond_t));

    //Initialize the condition variables for each event flag in the code
    pthread_cond_init(event_flags[0], NULL);
    pthread_cond_init(event_flags[1], NULL);
    pthread_cond_init(event_flags[2], NULL);
    pthread_cond_init(event_flags[3], NULL);
    pthread_cond_init(event_flags[4], NULL);
    pthread_cond_init(event_flags[5], NULL);
    pthread_cond_init(event_flags[6], NULL);
    pthread_cond_init(event_flags[7], NULL);
    pthread_cond_init(event_flags[8], NULL);
    pthread_cond_init(event_flags[9], NULL);    

    //Set thread attributes that all threads will utilize
    pthread_attr_init(&thread_attr); 
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED); //We must specify to the thread that the thread is explicitly setting its own schedule

    //Create the schedule parameter for the main thread and set it
    struct sched_param main_sp;
    main_sp.sched_priority = 1;
    pthread_t main_thread = pthread_self();
    pthread_setschedparam(main_thread, SCHED_FIFO, &main_sp);

    //Create the keyboard thread to be used to read input
    pthread_t keyboard_thread_id;
    
    //Create a struct of arguments that the keyboard thread needs
    struct keyboard_reader_args* kra = malloc(sizeof(struct keyboard_reader_args));
    kra->activate_condition = activate_condition;
    kra->terminate = terminated;
    kra->events = event_flags;
    kra->path_to_device_file = KEYBOARD_EVENT_DEV;
    kra->stop_sleep = stop_sleep;
    
    //Set the scheduling parameters of the keyboard threads and add it to the list of threads
    struct sched_param keyboard_sp;
    keyboard_sp.sched_priority = 1;
    pthread_create(&keyboard_thread_id, &thread_attr, keyboard_reader, kra);
    pthread_setschedparam(keyboard_thread_id, SCHED_FIFO, &keyboard_sp);
    thread_ids[num_thread_id++] = keyboard_thread_id;

    //Read the thread model given in thread_model.h and create threads
    int thread_num = 0;
    while(thread_num < NUM_THREADS)
    {
        //Initialize local variable for thread creation
        pthread_t thread_id;
        struct sched_param sp;
        sp.sched_priority = threads[thread_num].priority;

        //Create periodic thread in this section
        if(threads[thread_num].task_type == 0)
        {
            //Allocate memory for a struct to pass arguments to the thread
            pti = malloc(sizeof(struct periodic_task_information));
            pti->activate_condition = activate_condition;
            pti->stop_sleep = stop_sleep;
            pti->loop_iter = threads[thread_num].loop_iter;
            pti->task_num = threads[thread_num].task_num;
            pti->period = threads[thread_num].period;
            pti->thread_mutex = mutex[threads[thread_num].mutex_num];
            pti->terminate = terminated;

            //Create the actual thread and begin running it
            //Place the thread in the memory holder to delete later
            pthread_create(&thread_id, &thread_attr, periodic_task, pti);
            pti_memory_holder[num_pti++] = pti;
        }
        else
        {
            //Allocate memory for aperiodic thread arguments
            ati = malloc(sizeof(struct aperiodic_task_information));
            ati->activate_condition = activate_condition;
            ati->loop_iter = threads[thread_num].loop_iter;
            ati->task_num = threads[thread_num].task_num;
            ati->thread_mutex = mutex[threads[thread_num].mutex_num];
            ati->terminate = terminated;
            ati->event_occurred = event_flags[threads[thread_num].event_key];

            //Create the actual thread and begin runnig it
            //Place the thread in memory holder to delet later
            pthread_create(&thread_id, &thread_attr, aperiodic_task, ati);
            ati_memory_holder[num_ati++] = ati;
        }

        //Set the schedule parameters of the newly created thread
        pthread_setschedparam(thread_id, SCHED_FIFO, &sp);

        //Place the thread in the thread id array for thread cleanup later
        thread_ids[num_thread_id++] = thread_id;
        thread_num++;
    }

    printf("Program loaded and ready for activation...\n");

    //Create a mutex for the main thread to wait for activation call from user
    pthread_mutex_t main_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_wait(activate_condition, &main_mutex);

    //Sleep until time is up. This is when the other threads will continue doing their tasks acordingly
    millisleep(TOTAL_TIME);

    //After this point, we begin cleanup and terminate the program

    //Set terminated to true for threads to know to begin terminating
    (*terminated) = true;

    //Broadcast to the aperiodic threads that an event has occurred and it's time to terminate
    index = 0;
    while(index < NUM_EVENTS)
    {
        pthread_cond_broadcast(event_flags[index++]);
    } 

    //Broadcast to the periodic sleeping threads to stop sleeping and start terminating
    pthread_cond_broadcast(stop_sleep);

    //Join the terminating threads we created earlier
    while(num_thread_id > 0)
    {
        if(num_thread_id - 1 == 0)
        {
            //The keyboard thread we must cancel since it gets stuck on the read function
            pthread_cancel(thread_ids[num_thread_id - 1]);
        }

        else
        {
            pthread_join(thread_ids[num_thread_id - 1], NULL);
        }

        num_thread_id--;
    }

    //Destroy the condition and attributes we created earlier for the threads
    pthread_attr_destroy(&thread_attr);
    pthread_cond_destroy(activate_condition);
    pthread_cond_destroy(stop_sleep);

    //Free the terminated variable
    free((void*)terminated);

    //Cleanup the condition variables for the event flags
    pthread_cond_destroy(event_flags[0]);
    pthread_cond_destroy(event_flags[1]);
    pthread_cond_destroy(event_flags[2]); 
    pthread_cond_destroy(event_flags[3]);
    pthread_cond_destroy(event_flags[4]);
    pthread_cond_destroy(event_flags[5]);
    pthread_cond_destroy(event_flags[6]); 
    pthread_cond_destroy(event_flags[7]); 
    pthread_cond_destroy(event_flags[8]); 
    pthread_cond_destroy(event_flags[9]); 

    //Free the double pointer for event flags
    free((void*)event_flags);

    //Keyboard argument struct pointer freed
    free((void*)kra);

    //Iterate through all mutexes and destroy them accordingly
    index = 0;
    while(index < NUM_MUTEXES)
    {
        free((void*)mutex[index]);
        index++;
    }

    //Free the double pointer mutex itself
    free((void*)mutex);

    //Free all the struct arguments made for the periodic thread
    while(num_pti > 0)
    {
        free((void*)pti_memory_holder[num_pti - 1]);
        num_pti--;
    }

    //Free all the struct arguments made for the aperiodic thread
    while(num_ati > 0)
    {
        free((void*)ati_memory_holder[num_ati - 1]);
        num_ati--;
    }

    return 0;
}
