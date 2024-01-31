
#include <Windows.h>
#include <cstdio>
#include <cstring>

#include "pnut.h"
#include "cbot.h"

extern "C"
{
// #include "stopwatch.h"
#include "timeanalyzer.h"
}



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(TMAN_BASIC, "Test all timeanalyzer functions.")
{
    int stat = timeanalyzer_Init(55);
    UT_EQUAL(stat, CBOT_ERR_NO_ERR);


    // Start timing capture.
    // void timeanalyzer_Arm(void);


    // Capture a time value. return pointer to results if completed else NULL.
    // time_results_t* timeanalyzer_Grab(void);


    // void timeanalyzer_Reset(void);


    return 0;
}
