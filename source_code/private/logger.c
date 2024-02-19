
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
//#include <sys/time.h>

#include "diagnostics.h"
#include "cbot.h"
#include "cbot_internal.h"
#include "logger.h"


//---------------- Private Declarations ------------------//

#define LOG_LINE_LEN 500

/// Logging level.
static log_level_t p_level = LVL_INFO;

/// Output.
static FILE* p_fp = NULL;

/// Dawn of time.
static long long p_start_tick;

/// The performance counter scale.
static long long p_ticks_per_sec;

/// Translate to human.
/// @param level to translate.
/// @return string version
static const char* p_XlatLogLevel(log_level_t level);


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int logger_Init(FILE* fp)
{
    VAL_PTR(fp, CBOT_ERR_ARG_NULL);
    p_fp = fp;

    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    p_ticks_per_sec = f.QuadPart;
    QueryPerformanceCounter(&f);
    p_start_tick = f.QuadPart;

    return CBOT_ERR_NO_ERR;
}

//--------------------------------------------------------//
int logger_SetFilters(log_level_t level)
{
    p_level = level;
    return CBOT_ERR_NO_ERR;
}

//--------------------------------------------------------//
int logger_Log(log_level_t level, int line, const char* format, ...)
{
    VAL_PTR(format, CBOT_ERR_ARG_NULL);

    if (p_fp != NULL)
    {
        static char buff[LOG_LINE_LEN];

        // Check filters.
        if(level >= p_level)
        {
            va_list args;
            va_start(args, format);
            // The content.
            vsnprintf(buff, LOG_LINE_LEN-1, format, args);
            va_end(args);

            // Timestamp.
            LARGE_INTEGER f;
            QueryPerformanceCounter(&f);
            long long elapsed_ticks = f.QuadPart - p_start_tick;
            double sec = (double)elapsed_ticks / p_ticks_per_sec;

            fprintf(p_fp, "%03.6f %s %d %s\n", sec, p_XlatLogLevel(level), line, buff);
        }
    }

    return CBOT_ERR_NO_ERR;
}


//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
const char* p_XlatLogLevel(log_level_t level)
{
    static char buff[20];
    switch(level)
    {
        case LVL_INFO:  strcpy(buff,  "INF"); break;
        case LVL_TRACE: strcpy(buff,  "TRC"); break;
        case LVL_DEBUG: strcpy(buff,  "DBG"); break;
        case LVL_ERROR: strcpy(buff,  "ERR"); break;
        default: snprintf(buff, sizeof(buff)-1, "???%d", level); break;
    }
    return buff;
}    
