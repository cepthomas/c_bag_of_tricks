#include <Windows.h> //timeapi.h
#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "stopwatch.h"
#include "mathutils.h"
}


// TODO3 This doesn't actually test the stopwatch but rather the accuracy of Sleep().



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STOPWATCH_BASIC, "Test all stopwatch functions.")
{
    // What do we know?
    TIMECAPS tcaps;
    MMRESULT res = timeGetDevCaps(&caps, sizeof(caps));
    UT_INFO("wPeriodMin", tcaps.wPeriodMin);
    UT_INFO("wPeriodMax", tcaps.wPeriodMax);

    // Single capture accuracy.
    stopwatch_Init();
    UT_CLOSE(stopwatch_ElapsedMsec(), 0.0, 0.01);

    Sleep(250);
    UT_CLOSE(stopwatch_ElapsedMsec(), 250.0, 10.0);

    stopwatch_Reset();
    Sleep(500);
    UT_CLOSE(stopwatch_ElapsedMsec(), 500.0, 10.0);

    stopwatch_Reset();
    Sleep(1000);
    UT_CLOSE(stopwatch_ElapsedMsec(), 1000.0, 10.0);

    stopwatch_Reset();
    UT_CLOSE(stopwatch_ElapsedMsec(), 0.0, 0.01);

    ///// Repeat precision.
    const int NUM = 20;
    double vals[NUM];
    for (int i = 0;i < NUM; i++)
    {
        stopwatch_Reset();
        Sleep(100);
        vals[i] = stopwatch_ElapsedMsec();
    }

    stat_results_t res;
    mathutils_CalcDouble(vals, NUM, &res);
    UT_EQUAL(res.num_vals, NUM);
    UT_CLOSE(res.mean, 100.0, 0.01);
    UT_CLOSE(res.min, 100.0, 0.01);
    UT_CLOSE(res.max, 100.0, 0.01);
    UT_CLOSE(res.sd, 100.0, 0.01);

    return 0;
}
