#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "common.h"
#include "logger.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(LOGGER_XXX, "Test all logger functions.")
{
    common_Init();

// /// Log categories - from user.
// typedef enum
// {
//     LOG_INIT     = 1 << 8,
//     LOG_DESTROY  = 1 << 9,
//     LOG_LOOK     = 1 << 10,  ///> make it stand out in file
//     LOG_ALL_CATS = 0xFFFFFF00
// } log_cat_t;




// int logger_Init(FILE* fp);
// int logger_SetFilters(log_level_t level, log_cat_t cat);
// int logger_Destroy(void);
// void logger_Log(loglvl_t level, int cats, int line, const char* format, ...);

// #define LOG_BASE(level) logger_Log(level, cats, __LINE__, fmt, __VA_ARGS__);
// #define LOG_ERR(cats, fmt, ...)   LOG_BASE(loglvl_t.LOG_ERR);
// #define LOG_INFO(cats, fmt, ...)  LOG_BASE(loglvl_t.LOG_INFO);
// #define LOG_DEBUG(cats, fmt, ...) LOG_BASE(loglvl_t.LOG_DEBUG);

    return 0;
}    

