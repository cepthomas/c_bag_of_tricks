
#include <time.h>
#include <sys/time.h>
#include "common.h"


//---------------- Private Declarations ------------------//

/// For timing.
double p_start_time;

//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int common_Init()
{
    p_start_time = common_GetElapsedSec();
    return 0;
}

//--------------------------------------------------------//
double common_GetElapsedSec()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    double sec = tv.tv_sec + tv.tv_usec / 1000000;
    return sec;
}