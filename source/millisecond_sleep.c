#include <time.h>
#include <unistd.h>

void millisleep(long long ms)
{
    struct timespec time; //Holds the millisecond sleep time

    time.tv_sec = ms / 1000; //Take milliseconds and break it down to seconds.
    time.tv_nsec = (ms % 1000) * 1000000; //Remove the number of seconds within the milliseconds and convert remainder to nanoseconds

    nanosleep(&time, &time); //Time struct is placed within the nanosleep function to run that amount of time
}