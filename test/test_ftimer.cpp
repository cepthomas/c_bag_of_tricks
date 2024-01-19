
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>

#include "pnut.h"

extern "C"
{
#include "stopwatch.h"
#include "ftimer.h"
}


static double p_last_msec = 0.0;
#define TEST_COUNT 100
double p_test_res_1[TEST_COUNT]; // driver says
double p_test_res_2[TEST_COUNT]; // I say
int p_test_index = 0;


//-------------------------------------------------------//
void PeriodicInterruptFunc(double msec)
{
    if(p_test_index < TEST_COUNT)
    {
        double em = stopwatch_TotalElapsedMsec();
        p_test_res_1[p_test_index] = msec;
        p_test_res_2[p_test_index] = em - p_last_msec;
        p_last_msec = em;
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
    stopwatch_Init();
    
    // Happy path.
    unsigned res = 5;
    UT_EQUAL(ftimer_Init(PeriodicInterruptFunc, res), 0);

    // Grab the stopwatch time.
    p_last_msec = stopwatch_TotalElapsedMsec();

    // Go.
    UT_EQUAL(ftimer_Run(17), 0);

    int timeout = 3;
    while(ftimer_IsRunning())// && timeout > 0)
    {
        sleep(1);
        timeout--;
    }

    ftimer_Destroy();

    // Check what happened.
    double vmin_1 = 1000;
    double vmax_1 = 0;
    double vmin_2 = 1000;
    double vmax_2 = 0;

    for(int i = 0; i < TEST_COUNT; i++)
    {
        double v = p_test_res_1[i];
        vmin_1 = v < vmin_1 ? v : vmin_1;
        vmax_1 = v > vmax_1 ? v : vmax_1;

        v = p_test_res_2[i];
        vmin_2 = v < vmin_2 ? v : vmin_2;
        vmax_2 = v > vmax_2 ? v : vmax_2;

        //printf("%g\n", p_test_res[i]);
    }

    //printf("max_1:%g min_1:%g max_2:%g min_2:%g\n", vmax_1, vmin_1, vmax_2, vmin_2);

    return 0;
}

