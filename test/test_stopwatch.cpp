
//#include <Windows.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "stopwatch.h"
}



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STOPWATCH_BASIC, "Test all stopwatch functions.")
{
    // Happy path.
    stopwatch_Init();
    UT_CLOSE(stopwatch_ElapsedMsec(), 0.0, 0.01);

    stopwatch_Reset();
    UT_CLOSE(stopwatch_ElapsedMsec(), 0.0, 0.01);

    sleep(1);

    UT_CLOSE(stopwatch_ElapsedMsec(), 1000.0, 5.0); // because sleep() is sloppy TODO2 still off a bit on first one.

    return 0;
}
