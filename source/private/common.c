
#include <time.h>
#include <sys/time.h>

#include "common.h"


//---------------- Private Declarations ------------------//

/// For timing.
double p_start_time = 0;

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
    double current = common_GetCurrentSec();
    if(p_start_time == 0)
    {
        p_start_time = current;
    }
    double sec = current - p_start_time;
    return sec;
}

//--------------------------------------------------------//
double common_GetCurrentSec()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    double sec = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    return sec;
}

//--------------------------------------------------------//
void common_MemFail(int line, const char* file)
{
    logger_Log(LVL_ERROR, CAT_MEM, line, "Alloc failure: %s", file);
}
