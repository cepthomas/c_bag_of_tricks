#include <Windows.h>
#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "logger.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(LOGGER_FILE, "Test logger to file.")
{
    // Turn on all.
    UT_EQUAL(logger_SetFilters(LVL_DEBUG), 0);

    // Start logging stuff.
    LOG_ERROR("This should be an ERROR 99:%d", 99);

    Sleep(1000);
    LOG_INFO("INFO One second later... :%d", 555);

    Sleep(1000);
    LOG_DEBUG("DEBUG Two seconds later... :%d", 71717);

    LOG_TRACE("This should not appear");

    return 0;
}    
