
#include <windows.h>
#include <stdio.h>
#include "ftimer.h"


//---------------- Private Declarations ------------------//

/// Registered client callback.
static ftimer_InterruptFunc_t p_interrupt_func = NULL;

/// Requested pulse period in msec.
static unsigned p_period = 0;

/// Resolution of p_period.
static unsigned p_ft_res = 10;

/// System mmtimer resource.
static MMRESULT p_sys_handle = 0;

/// System mmtimer callback. Ignore all args.
static void CALLBACK p_TimerCallback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

/// Where we are in p_period.
static double p_accum_msec = 0.0;

/// Timer status.
static int p_status = 0;

/// Timer status.
static bool p_running = false;

/// Simple stats.
static double p_tmin = 1000;

/// Simple stats.
static double p_tmax = 0;

/// Measuring.
static long long p_last_tick = -1;

/// The performance counter scale.
static double p_ticks_per_msec;


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int ftimer_Init(ftimer_InterruptFunc_t fp, unsigned ft_res)
{
    if(fp == NULL || ft_res < 1)
    {
        p_status = -2;
    }
    else
    {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        p_ticks_per_msec = (double)f.QuadPart / 1000.0;

        p_interrupt_func = fp;
        p_ft_res = ft_res;
    }

    p_tmin = 1000;
    p_tmax = 0;

    return p_status;
}

//--------------------------------------------------------//
int ftimer_Run(unsigned period)
{
    if(p_status == 0)
    {
        p_period = period;

        if(period > 0)
        {
            // Clean up if already running.
            if(p_sys_handle > 0)
            {
                timeKillEvent(p_sys_handle);
                p_sys_handle = 0;
            }

            p_running = true;
            p_accum_msec = 0.0;
            LARGE_INTEGER f;
            QueryPerformanceCounter(&f);
            p_last_tick = f.QuadPart;
            p_sys_handle = timeSetEvent(p_ft_res, 0, p_TimerCallback, 0, TIME_PERIODIC);
        }
        else // stop
        {
            if(p_sys_handle > 0)
            {
                timeKillEvent(p_sys_handle); //check return result != TIMERR_NOERROR
            }

            p_sys_handle = 0;
            p_status = -1;
            p_running = false;
            //printf("p_tmin:%g p_tmax:%g\n", p_tmin, p_tmax);
        }
    }

    return p_status;
}

//--------------------------------------------------------//
int ftimer_Destroy(void)
{
    if(p_sys_handle > 0)
    {
        timeKillEvent(p_sys_handle);
    }

    p_sys_handle = 0;
    p_period = 0;
    p_interrupt_func = NULL;
    p_status = 0;
    p_running = false;

    return p_status;
}

//--------------------------------------------------------//
bool ftimer_IsRunning(void)
{
    return p_running;    
}

//---------------- Private Implementation -------------//

//--------------------------------------------------------//
void CALLBACK p_TimerCallback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    if(p_running)
    {
        // When are we?
        LARGE_INTEGER f;
        QueryPerformanceCounter(&f);
        long long elapsed_ticks = f.QuadPart - p_last_tick;
        double msec = (double)(elapsed_ticks / p_ticks_per_msec);

        // // When are we?
        // double t = stopwatch_ElapsedMsec();

        // // Arm for next capture.
        // stopwatch_Reset();

        p_tmin = msec < p_tmin ? msec : p_tmin;
        p_tmax = msec > p_tmax ? msec : p_tmax;

        // double msec = msec - p_last_msec;
        // p_last_msec = msec;

        // Check for expirations.
        // p_current_msec += msec;
        p_accum_msec += msec;

        const double ALLOWANCE = 0.5; // msec

        if((p_period - p_accum_msec) < ALLOWANCE)
        {
            if(p_interrupt_func != NULL)
            {
                p_interrupt_func(p_accum_msec);
            }
            p_accum_msec = 0.0;
        }
    }
}
