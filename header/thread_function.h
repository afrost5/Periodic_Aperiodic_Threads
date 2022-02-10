#ifndef THREAD_FUNCTION
#define THREAD_FUNCTION
#include <stdint.h>

//Funciton to run a periodic task
void* periodic_task(void* thread_args);

//Function to run an aperiodic task
void* aperiodic_task(void* thread_args);

//Function for the computation of the tasks
void computation(uint64_t val);

#endif //THREAD_FUNCTION