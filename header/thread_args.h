#ifndef THREAD_ARGS_
#define THREAD_ARGS_

#include<pthread.h>

struct periodic_task_information
{
    int task_num;
    int* loop_iter;
    pthread_mutex_t* thread_mutex;
    int period;
    volatile bool* activate;
    volatile bool* terminate;
};

struct aperiodic_task_information
{
    int task_num;
    int* loop_iter;
    pthread_mutex_t* thread_mutex;
    volatile bool* event_occurred;
    volatile bool* activate;
    volatile bool* terminate;
};

struct keyboard_reader_args
{
    volatile bool** events;
    volatile bool* activate;
    volatile bool* terminate;
    char* path_to_device_file;
};

#endif //THREAD_ARGS_