
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>

#include "pnut.h"

extern "C"
{
#include "mathutils.h"
#include "ftimer.h"
}

// The current performance counter scale.
static double p_ticks_per_msec;

// Periodic time.
static long long p_last_tick;

#define TEST_COUNT 100
double p_test_res[TEST_COUNT];
int p_test_index = 0;


//--------------------------------------------------------//
long long p_CurrentTick(void)
{
    LARGE_INTEGER ts;
    QueryPerformanceCounter(&ts);
    return ts.QuadPart;
}


//-------------------------------------------------------//
void PeriodicInterruptFunc(double msec)
{
    if(p_test_index < TEST_COUNT)
    {
        long long current_tick = p_CurrentTick();
        long long elapsed_ticks = current_tick - p_last_tick;
        double msec = (double)elapsed_ticks / p_ticks_per_msec;
        p_test_res[p_test_index] = msec;
        p_last_tick = current_tick;
        p_test_index++;
    }
    else
    {
        // Stop.
        ftimer_Run(0);
    }
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(FTIMER_BASIC, "Test all ftimer functions.")
{
    // stopwatch_Init();
    
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    p_ticks_per_msec = (double)f.QuadPart / 1000.0;

    // Set resolution to best.
    int stat = ftimer_Init(PeriodicInterruptFunc, 1);
    UT_EQUAL(stat, 0);

    p_last_tick = p_CurrentTick();

    // Go. 10 msec period.
    UT_EQUAL(ftimer_Run(10), 0);

    int timeout = 30; // safety
    while(ftimer_IsRunning() && timeout > 0)
    {
        Sleep(100);
        timeout--;
    }

    UT_GREATER(timeout, 0);

    ftimer_Destroy();

    // for(int i = 0; i < p_test_index; i++)
    // {
    //     printf("%d:%f\n", i, p_test_res[i]);
    // }

    // Check what happened. TODO1 doesn't work.
    stat_results_t res;
    int num = p_test_index;
    mathutils_CalcStats(p_test_res, num, &res);
    UT_INFO("Num vals", res.num_vals);
    UT_EQUAL(res.num_vals, num);
    UT_CLOSE(res.mean, 5.00, 0.01);
    UT_CLOSE(res.min, 5.00, 0.01);
    UT_CLOSE(res.max, 5.00, 0.01);
    UT_CLOSE(res.sd, 5.00, 0.01);

    return 0;
}

