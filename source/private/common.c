
#include <time.h>
#include <sys/time.h>
#include <assert.h>

#include "common.h"


//---------------- Private Declarations ------------------//

/// For timing.
double p_start_time = 0;

/// Returns the current number of seconds since the epoch.
/// @return The sec.
double p_GetCurrentSec(void);


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int common_Init()
{
    p_start_time = p_GetCurrentSec();
    return 0;
}

//--------------------------------------------------------//
double common_GetElapsedSec()
{
    assert(p_start_time != 0);

    double current = p_GetCurrentSec();
    double sec = current - p_start_time;
    return sec;
}

//--------------------------------------------------------//
void common_MemFail(int line, const char* file)
{
    logger_Log(LVL_ERROR, CAT_MEM, line, "Alloc/free failure: %s", file);
    exit(1);
}

//---------------- Private Implementation -------------//

//--------------------------------------------------------//
double p_GetCurrentSec()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    double sec = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    return sec;
}
