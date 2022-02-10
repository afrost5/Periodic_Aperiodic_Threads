#ifndef THREAD_ARGS_
#define THREAD_ARGS_

#include<pthread.h>

//Struct that holds periodic task arguments for the thread
struct periodic_task_information
{
    pthread_mutex_t* thread_mutex; //Mutex needed for critical section
    pthread_cond_t* activate_condition; //Condition to activate the thread
    pthread_cond_t* stop_sleep; //Condition to wake thread up from sleeping
    int task_num; //Hold the value of the task number
    int* loop_iter; //Holds the number of computations for the iteration
    int period; //Period of time the thread will keep running
    volatile bool* terminate; //Boolean to tell thread to go to termination state
};

//Struct that holds aperiodic task arguments for the thread
struct aperiodic_task_information
{
    pthread_mutex_t* thread_mutex; //Mutex needed for critical section
    pthread_cond_t* event_occurred; //Condition to tell thread that an event has occurred
    pthread_cond_t* activate_condition; //Condition to activate the thread
    int task_num; //Hold the value of the task number
    int* loop_iter; //Holds the number of computations for the iteration
    volatile bool* terminate; //Boolean to tell thread to go to termination state
};

//Struct that holds keyboard reader  arguments for the thread
struct keyboard_reader_args
{
    pthread_cond_t* activate_condition; //Condition to activate the thread
    pthread_cond_t* stop_sleep; //Condition to wake thread up from sleeping
    pthread_cond_t** events; //Condition array to activate the events for other thread
    volatile bool* terminate; //Boolean to tell thread to go to termination state
    char* path_to_device_file; //Device file path to read the keyboard input
};

#endif //THREAD_ARGS_