
#include <windows.h>
#include <stdio.h>

#include "cbot.h"
#include "cbot_internal.h"
#include "ftimer.h"

// TODO3 Is measured performance really better than native?


//---------------- Private Declarations ------------------//

/// Registered client callback.
static ftimer_InterruptFunc_t p_interrupt_func = NULL;

/// Requested pulse period in msec.
static double p_period = 0;

/// Resolution of p_period.
static unsigned p_ft_res = 0;

/// System mmtimer resource.
static MMRESULT p_sys_handle = 0;

/// System mmtimer callback. Ignore all args.
static void CALLBACK p_TimerCallback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

/// Where we are in p_period.
static double p_accum_msec = 0.0;

/// Timer state. 0=stopped 1=running -1=invalid.
static int p_state = -1;

/// Measuring.
static long long p_last_tick = -1;

/// The performance counter scale.
static double p_ticks_per_msec;


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int ftimer_Init(ftimer_InterruptFunc_t fp, unsigned ft_res)
{
    VAL_PTR(fp, CBOT_ERR_ARG_NULL);
    int stat = CBOT_ERR_NO_ERR;

    if(ft_res < 1)
    {
        stat = EINVAL;
        p_ft_res = 0;
        p_state = -1;
    }
    else
    {
        p_ft_res = ft_res;
        p_state = 0;
    }

    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    p_ticks_per_msec = (double)f.QuadPart / 1000.0;
    p_interrupt_func = fp;
    return stat;
}

//--------------------------------------------------------//
int ftimer_Run(double period)
{
    int stat = CBOT_ERR_NO_ERR;

    if(p_state >= 0)
    {
        p_period = period;

        if(p_period > 0)
        {
            // Clean up if already running.
            if(p_sys_handle > 0)
            {
                timeKillEvent(p_sys_handle);
                p_sys_handle = 0;
            }

            p_state = 1;
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
            p_state = 0;
        }
    }
    else
    {
        stat = EINVAL;
    }

    return stat;
}

//--------------------------------------------------------//
int ftimer_Destroy(void)
{
    int stat = CBOT_ERR_NO_ERR;

    if(p_sys_handle > 0)
    {
        timeKillEvent(p_sys_handle);
    }

    p_sys_handle = 0;
    p_period = 0;
    p_interrupt_func = NULL;
    p_state = -1;

    return stat;
}

//--------------------------------------------------------//
bool ftimer_IsRunning(void)
{
    return p_state == 1;    
}

//---------------- Private Implementation -------------//

//--------------------------------------------------------//
void CALLBACK p_TimerCallback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    if(p_state == 1)
    {
        // When are we?
        LARGE_INTEGER f;
        QueryPerformanceCounter(&f);
        long long elapsed_ticks = f.QuadPart - p_last_tick;
        p_last_tick = f.QuadPart;
        double msec = (double)(elapsed_ticks / p_ticks_per_msec);

        // Check for expiration. Allow values +- resolution / 2.
        p_accum_msec += msec;
        if((p_period - p_accum_msec) < (double)p_ft_res / 2)
        {
            if(p_interrupt_func != NULL)
            {
                p_interrupt_func(p_accum_msec);
            }
            p_accum_msec = 0.0;
        }
    }
}
