
#include <windows.h>
#include <stdio.h>
#include "stopwatch.h"



//---------------- Private Declarations ------------------//

/// Dawn of time.
static long long p_start_tick;

/// Periodic time.
static long long p_last_tick;

/// The current performance counter scale.
static double p_ticks_per_msec;

/// The current performance counter value.
/// @return ticks
static long long p_CurrentTick(void);


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
void stopwatch_Init(void)
{
    p_start_tick = 0;
    p_last_tick = 0;

    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    p_ticks_per_msec = (double)f.QuadPart / 1000.0;
    stopwatch_Reset();
}

//--------------------------------------------------------//
void stopwatch_Reset(void)
{
    p_last_tick = p_CurrentTick();
}

//--------------------------------------------------------//
double stopwatch_ElapsedMsec(void)
{
    long long elapsed_ticks = p_CurrentTick() - p_last_tick;
    double msec = (double)elapsed_ticks / p_ticks_per_msec;
    return msec;
}

//--------------------------------------------------------//
double stopwatch_TotalElapsedMsec(void)
{
    long long elapsed_ticks = p_CurrentTick() - p_start_tick;
    double msec = (double)elapsed_ticks / p_ticks_per_msec;
    return msec;
}

//---------------- Private Implementation -------------//

//--------------------------------------------------------//
long long p_CurrentTick(void)
{
    LARGE_INTEGER ts;
    QueryPerformanceCounter(&ts);
    return ts.QuadPart;
}
