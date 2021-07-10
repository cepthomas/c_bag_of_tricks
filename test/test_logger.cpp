#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "pnut.h"

extern "C"
{
#include "common.h"
#include "logger.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(LOGGER_FILE, "Test logger to file.")
{
    common_Init();

    system("del /q log_test.txt");

    FILE* fp = fopen("log_test.txt", "w");
    UT_NOT_NULL(fp);

    UT_EQUAL(logger_Init(fp), 0);

    // Turn on all.
    UT_EQUAL(logger_SetFilters(LVL_DEBUG, CAT_ALL), 0);

    // Start logging stuff.
    LOG_ERROR(CAT_INIT, "Hello error 99:%d", 99);

    sleep(1);
    LOG_INFO(CAT_DESTROY, "One second later... info:%d", 555);

    sleep(1);
    LOG_DEBUG(CAT_LOOK, "Two second later... debug:%d", 71717);

    // Specific filters.
    UT_EQUAL(logger_SetFilters(LVL_INFO, CAT_LOOK), 0);

    LOG_INFO(CAT_DESTROY, "This should not appear");
    LOG_INFO(CAT_LOOK, "This should appear");
    LOG_ERROR(CAT_LOOK, "This should not appear");

    UT_EQUAL(logger_Destroy(), 0);

    return 0;
}    

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(LOGGER_STDOUT, "Test logger to stdout.")
{
    common_Init();

    UT_EQUAL(logger_Init(NULL), 0);

    // Turn on all.
    UT_EQUAL(logger_SetFilters(LVL_DEBUG, CAT_ALL), 0);

    // Start logging stuff.
    LOG_ERROR(CAT_INIT, "Hello stdout 99:%d", 1000000);

    sleep(1);
    LOG_INFO(CAT_ENTRY, "One second later... stdout:%d", 888);

    sleep(1);
    LOG_DEBUG(CAT_LOOK, "Two second later... stdout:%d", 333);

    UT_INFO("Need to look in terminal!!", "OK!!");

    UT_EQUAL(logger_Destroy(), 0);

    return 0;
}    
