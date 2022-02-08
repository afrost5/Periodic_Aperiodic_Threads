#include <sys/time.h>
#include <time.h>

#include "../header/time_stamp.h"

long long milliseconds_time()
{
    struct timeval time_stamp;
    gettimeofday(&time_stamp, NULL);
    return((time_stamp.tv_sec * 1000) + (time_stamp.tv_usec / 1000));
}