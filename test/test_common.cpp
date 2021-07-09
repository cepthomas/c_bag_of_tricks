#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "pnut.h"

extern "C"
{
#include "common.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(COMMON_TIME, "Test time common functions.")
{
    UT_EQUAL(common_Init(), 0);

    UT_CLOSE(common_GetElapsedSec(), 0.0, 0.01);

    sleep(1);
    UT_CLOSE(common_GetElapsedSec(), 1.0, 0.01);

    sleep(1);
    UT_CLOSE(common_GetElapsedSec(), 2.0, 0.01);

    return 0;
}    
