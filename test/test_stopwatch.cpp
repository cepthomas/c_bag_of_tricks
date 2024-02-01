#include <Windows.h> //timeapi.h
#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "stopwatch.h"
#include "mathutils.h"
}


// TODO3 This doesn't actually test the stopwatch but rather the accuracy of Sleep() ~15.6ms. Oh well.
const double TRES = 16; 


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STOPWATCH_BASIC, "Test all stopwatch functions.")
{
    // What do we know? 1 -> 1000000.
    // TIMECAPS tcaps;
    // timeGetDevCaps(&tcaps, sizeof(tcaps));
    // UT_INFO("wPeriodMin", tcaps.wPeriodMin);
    // UT_INFO("wPeriodMax", tcaps.wPeriodMax);

    // Single capture accuracy.
    stopwatch_Init();
    UT_CLOSE(stopwatch_ElapsedMsec(), 0.0, 0.01);

    Sleep(250);
    UT_CLOSE(stopwatch_ElapsedMsec(), 250.0, TRES);

    stopwatch_Reset();
    Sleep(500);
    UT_CLOSE(stopwatch_ElapsedMsec(), 500.0, TRES);

    stopwatch_Reset();
    Sleep(1000);
    UT_CLOSE(stopwatch_ElapsedMsec(), 1000.0, TRES);

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

    stat_results_t stats;
    mathutils_CalcStats(vals, NUM, &stats);
    UT_EQUAL(stats.num_vals, NUM);
    UT_CLOSE(stats.mean, 100.0, TRES);
    UT_CLOSE(stats.min, 100.0, TRES);
    UT_CLOSE(stats.max, 100.0, TRES);
    UT_CLOSE(stats.sd, 0.6, 0.1);

    return 0;
}
