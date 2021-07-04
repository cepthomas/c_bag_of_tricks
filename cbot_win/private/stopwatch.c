
#include <windows.h> //mmsystem.h

#include "stopwatch.h"


//---------------- Private --------------------------//

// ElapsedTicks;
// IsRunning;
// Start();
// Stop();


// This class uses high-resolution performance counter if installed hardware 
// does not support it. Otherwise, the class will fall back to DateTime class
// and uses ticks as a measurement.

static const long TICKS_PER_MSEC = 10000;
static const long TICKS_PER_SEC = TICKS_PER_MSEC * 1000;

static long p_elapsed_time;

static long p_start_time;

static bool p_running = false;

// "Frequency" stores the frequency of the high-resolution performance counter, 
// if one exists. Otherwise it will store TICKS_PER_SEC. 
// The frequency cannot change while the system is running,
// so we only need to initialize it once. 
// static long Frequency;

static bool p_valid = false;

// performance-counter frequency, in counts per ticks.
// This can speed up conversion from high frequency performance-counter to ticks. 
static long p_tick_frequency;  



long p_ElapsedDateTimeTicks();

long p_GetTimestamp(void);

long p_RawElapsedTicks(void);


//profileapi.h
//BOOL QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);
// typedef union _LARGE_INTEGER {
//   struct {
//     DWORD LowPart;
//     LONG  HighPart;
//   } DUMMYSTRUCTNAME;
//   struct {
//     DWORD LowPart;
//     LONG  HighPart;
//   } u;
//   LONGLONG QuadPart;
// } LARGE_INTEGER;
//The LARGE_INTEGER structure is actually a union.
//If your compiler has built-in support for 64-bit integers, use the QuadPart member to store the 64-bit integer.
//Otherwise, use the LowPart and HighPart members to store the 64-bit integer.
// also ULARGE_INTEGER


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int stopwatch_Init(void)
{
    int status = 0;

    LARGE_INTEGER f;
    if(QueryPerformanceFrequency(&f))
    {
        p_valid = true;
        p_tick_frequency = TICKS_PER_SEC / f.QuadPart;
    }
    else
    {
        p_valid = false;
        status = 1;
    }   

    //Reset(); ==
    p_elapsed_time = 0;
    p_running = false;
    p_start_time = 0;

    return status;
}

//--------------------------------------------------------//
int stopwatch_Start(void)
{
    int status = 0;

    // Calling start on a running Stopwatch is a no-op.
    if(!p_running)
    {
        p_start_time = p_GetTimestamp();                 
        p_running = true;
    }

    return status;
}

//--------------------------------------------------------//
int stopwatch_Stop(void)
{
    int status = 0;

    // Calling stop on a stopped Stopwatch is a no-op.
    if( p_running)
    {
        long end_ts = p_GetTimestamp();                 
        long elapsed = end_ts - p_start_time;
        p_elapsed_time += elapsed;
        p_running = false;

        if (p_elapsed_time < 0)
        {
            // When measuring small time periods the StopWatch.Elapsed* 
            // properties can return negative values.  This is due to 
            // bugs in the basic input/output system (BIOS) or the hardware
            // abstraction layer (HAL) on machines with variable-speed CPUs
            // (e.g. Intel SpeedStep).

            p_elapsed_time = 0;
        }
    }

    return status;
}

//--------------------------------------------------------//
bool stopwatch_IsRunning(void)
{ 
    return p_running;
}

//--------------------------------------------------------//
long stopwatch_ElapsedMilliseconds(void)
{ 
    return p_ElapsedDateTimeTicks() / TICKS_PER_MSEC;
}  

//--------------------------------------------------------//
long stopwatch_ElapsedTicks(void)
{ 
    return p_RawElapsedTicks();
}

//---------------- Private Implementation -------------//

//--------------------------------------------------------//
long p_ElapsedDateTimeTicks()
{
    long raw_ticks = p_RawElapsedTicks();

    // convert high resolution perf counter to DateTime ticks
    double dticks = raw_ticks;
    dticks *= p_tick_frequency;
    return (long)dticks;                        
}   


// Get the p_elapsed_time ticks.        
//--------------------------------------------------------//
long p_RawElapsedTicks(void)
{
    long time_elapsed = p_elapsed_time;

    if( p_running)
    {
        // If the StopWatch is running, add p_elapsed_time time since the Stopwatch is started last time. 
        long ts = p_GetTimestamp();                 
        long elapsedUntilNow = ts - p_start_time;
        time_elapsed += elapsedUntilNow;
    }
    return time_elapsed;
}   

//--------------------------------------------------------//
long p_GetTimestamp(void)
{
    LARGE_INTEGER ts;    
    QueryPerformanceCounter(&ts);
    return ts.QuadPart;
}
