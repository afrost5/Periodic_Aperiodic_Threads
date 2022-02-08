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
    const int NUM_EVENTS = 10;

    //<Shared by Threads>
    volatile bool* activated;
    volatile bool* terminated;
    volatile bool** event_flags;
    pthread_mutex_t** mutex;
  

    //<Local variables>
    pthread_attr_t thread_attr;

    //<Initialization>
    int num_pti = 0;
    int num_ati = 0;
    struct periodic_task_information* pti_memory_holder[NUM_THREADS];
    struct aperiodic_task_information* ati_memory_holder[NUM_THREADS];

    int num_thread_id = 0;
    pthread_t thread_ids[NUM_THREADS + 1];

    //Allocate mutex array for thread synchronization
    
    mutex = malloc(NUM_MUTEXES * sizeof(pthread_mutex_t*));
    
    //Initialize mutex array with individual mutexes
    int index = 0;
    while(index < NUM_MUTEXES)
    {
        mutex[index] = malloc(sizeof(pthread_mutex_t));
        index++;
    }


    activated = malloc(sizeof(bool));
    (*activated) = false;
        
    terminated = malloc(sizeof(bool));
    (*terminated) = false;

    event_flags = malloc(NUM_EVENTS * sizeof(bool*));
    event_flags[0] = malloc(sizeof(bool));
    event_flags[1] = malloc(sizeof(bool));
    event_flags[2] = malloc(sizeof(bool));
    event_flags[3] = malloc(sizeof(bool));
    event_flags[4] = malloc(sizeof(bool));
    event_flags[5] = malloc(sizeof(bool));
    event_flags[6] = malloc(sizeof(bool));
    event_flags[7] = malloc(sizeof(bool));
    event_flags[8] = malloc(sizeof(bool));
    event_flags[9] = malloc(sizeof(bool));

    *event_flags[0] = false;
    *event_flags[1] = false;
    *event_flags[2] = false;
    *event_flags[3] = false;
    *event_flags[4] = false;
    *event_flags[5] = false;
    *event_flags[6] = false;
    *event_flags[7] = false;
    *event_flags[8] = false;
    *event_flags[9] = false;

    //Thread attribute settings
    pthread_attr_init(&thread_attr); //Initializes it with default values
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);

    //Start keyboard thread...
    pthread_t keyboard_thread_id;

    struct keyboard_reader_args* kra = malloc(sizeof(struct keyboard_reader_args));
    kra->activate = activated;
    kra->terminate = terminated;
    kra->events = event_flags;
    kra->path_to_device_file = KEYBOARD_EVENT_DEV;

    pthread_create(&keyboard_thread_id, &thread_attr, keyboard_reader, kra);

    thread_ids[num_thread_id++] = keyboard_thread_id;

    //Create all the tasks...
    //Start task threads to wait for activation...
    int thread_num = 0;
    struct periodic_task_information* pti;
    struct aperiodic_task_information* ati;
    while(thread_num < NUM_THREADS)
    {
        pthread_t thread_id;
        struct sched_param sp;
        sp.sched_priority = threads[thread_num].priority;

        //0 periodic task
        //1 aperiodic task
        if(threads[thread_num].task_type == 0)
        {
            pti = malloc(sizeof(struct periodic_task_information));
            
            pti->activate = activated;
            pti->loop_iter = threads[thread_num].loop_iter;
            pti->task_num = threads[thread_num].task_num;
            pti->period = threads[thread_num].period;
            pti->thread_mutex = mutex[threads[thread_num].mutex_num];
            pti->terminate = terminated;

            pthread_create(&thread_id, &thread_attr, periodic_task, pti);

            pti_memory_holder[num_pti++] = pti;
        }
        else
        {
            ati = malloc(sizeof(struct aperiodic_task_information));
            
            ati->activate = activated;
            ati->loop_iter = threads[thread_num].loop_iter;
            ati->task_num = threads[thread_num].task_num;
            ati->thread_mutex = mutex[threads[thread_num].mutex_num];
            ati->terminate = terminated;
            ati->event_occurred = event_flags[threads[thread_num].event_key];

            pthread_create(&thread_id, &thread_attr, aperiodic_task, ati);

            ati_memory_holder[num_ati++] = ati;
        }

        pthread_setschedparam(thread_id, SCHED_FIFO, &sp);

        thread_ids[num_thread_id++] = thread_id;
        thread_num++;
    }

    printf("Program loaded and ready for activation...\n");
    while(!(*activated)); //Wait for activation...

    millisleep(TOTAL_TIME); //Wait until we time_out...
    (*terminated) = true; //Set terminated to true to stop all threads...

    //<Clean Up>
    while(num_thread_id > 0)
    {
        if(num_thread_id - 1 == 0)
        {
            pthread_cancel(thread_ids[num_thread_id - 1]);
        }

        else
        {
            pthread_join(thread_ids[num_thread_id - 1], NULL);
        }

        num_thread_id--;
    }

    pthread_attr_destroy(&thread_attr);

    free((void*)activated);
    free((void*)terminated);

    free((void*)event_flags[0]);
    free((void*)event_flags[1]);
    free((void*)event_flags[2]); 
    free((void*)event_flags[3]);
    free((void*)event_flags[4]); 
    free((void*)event_flags[5]);
    free((void*)event_flags[6]);
    free((void*)event_flags[7]);
    free((void*)event_flags[8]);
    free((void*)event_flags[9]);
    free((void*)event_flags);

    free((void*)kra);

    index = 0;
    while(index < NUM_MUTEXES)
    {
        free((void*)mutex[index]);
        index++;
    }
    free((void*)mutex);

    while(num_pti > 0)
    {
        free((void*)pti_memory_holder[num_pti - 1]);
        num_pti--;
    }

    while(num_ati > 0)
    {
        free((void*)ati_memory_holder[num_ati - 1]);
        num_ati--;
    }

    return 0;
}
