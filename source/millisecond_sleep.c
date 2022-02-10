#include <time.h>
#include <unistd.h>

//Function to make thread sleep for a number of milliseconds
void millisleep(long long ms)
{
    //Holds the millisecond sleep time
    struct timespec time; 

    //Take milliseconds and break it down to seconds.
    time.tv_sec = ms / 1000; 

    //Remove the number of seconds within the milliseconds and convert remainder to nanoseconds
    time.tv_nsec = (ms % 1000) * 1000000; 

    //Time struct is placed within the nanosleep function to run that amount of time
    nanosleep(&time, &time); 
}