/*
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include "../header/keyboard_thread.h"
#include "../header/thread_model.h"
#include "../header/thread_function.h"
#include "../header/thread_args.h"

//printf("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
//ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);

int main()
{
    const int NUM_EVENTS = 10;

    pthread_t thread_id_0;
    pthread_t thread_id_1;
    pthread_t keyboard_thread_id;
    pthread_attr_t thread_attr;

    pthread_attr_init(&thread_attr); //Initializes it with default values
    pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO); //Set the scheduling parameter as FIFO for the threads...

    bool* activated = malloc(sizeof(bool));
    *activated = false;

    bool* terminated = malloc(sizeof(bool));
    *terminated = false;

    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
    
    int loops[3] = {5000000,5000000,5000000};

    //int time_interval = 3000;

    volatile bool** event_flags;

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

    (*event_flags)[0] = false;
    (*event_flags)[1] = false;
    (*event_flags)[2] = false;
    (*event_flags)[3] = false;
    (*event_flags)[4] = false;
    (*event_flags)[5] = false;
    (*event_flags)[6] = false;
    (*event_flags)[7] = false;
    (*event_flags)[8] = false;
    (*event_flags)[9] = false;

    
    struct keyboard_reader_args* kra = malloc(sizeof(struct keyboard_reader_args));
    kra->activate = activated;
    kra->terminate = terminated;
    kra->events = event_flags;
    kra->path_to_device_file = KEYBOARD_EVENT_DEV;

    
    struct periodic_task_information* pti0 = malloc(sizeof(struct periodic_task_information));
    pti0->activate = activated;
    pti0->loop_iter = loops;
    pti0->task_num = 0;
    pti0->terminate = terminated;
    pti0->thread_mutex = mutex;
    pti0->period = time_interval;

    struct periodic_task_information* pti1 = malloc(sizeof(struct periodic_task_information));
    pti1->activate = activated;
    pti1->loop_iter = loops;
    pti1->task_num = 1;
    pti1->terminate = terminated;
    pti1->thread_mutex = mutex;
    pti1->period = time_interval;
    

    
    struct aperiodic_task_information* ati0 = malloc(sizeof(struct aperiodic_task_information));
    ati0->activate = activated;
    ati0->loop_iter = loops;
    ati0->task_num = 0;
    ati0->terminate = terminated;
    ati0->thread_mutex = mutex;
    ati0->event_occurred = event_flags[0];

    struct aperiodic_task_information* ati1 = malloc(sizeof(struct aperiodic_task_information));
    ati1->activate = activated;
    ati1->loop_iter = loops;
    ati1->task_num = 1;
    ati1->terminate = terminated;
    ati1->thread_mutex = mutex;
    ati1->event_occurred = event_flags[1];
    

    //pthread_create(&thread_id_0, &thread_attr, periodic_task, pti0);
    //pthread_create(&thread_id_1, &thread_attr, periodic_task, pti1);

    pthread_create(&thread_id_0, &thread_attr, aperiodic_task, ati0);
    pthread_create(&thread_id_1, &thread_attr, aperiodic_task, ati1);
   
    pthread_create(&keyboard_thread_id, &thread_attr, keyboard_reader, kra);

    pthread_join(thread_id_0, NULL);
    pthread_join(thread_id_1, NULL);

    return 0;
}
*/