#ifndef THREAD_FUNCTION
#define THREAD_FUNCTION
#include <stdint.h>

void* periodic_task(void* thread_args);
void* aperiodic_task(void* thread_args);
void computation(uint64_t val);

#endif //THREAD_FUNCTION