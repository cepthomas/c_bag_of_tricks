
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

/// Logging level.
static log_level_t p_level = LVL_INFO;

/// Logging filters.
static log_cat_t p_cat = CAT_ALL;

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

/// Translate to human.
/// @param cat to translate.
/// @return string version
static const char* p_XlatLogCat(log_cat_t cat);


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
int logger_SetFilters(log_level_t level, log_cat_t cat)
{
    p_level = level;
    p_cat = cat;
    return CBOT_ERR_NO_ERR;
}

//--------------------------------------------------------//
int logger_Log(log_level_t level, log_cat_t cat, int line, const char* format, ...)
{
    VAL_PTR(format, CBOT_ERR_ARG_NULL);

    if (p_fp != NULL)
    {
        #define LOG_LINE_LEN 100
        static char buff[LOG_LINE_LEN];

        // Check filters.
        if((level >= p_level) && (cat & p_cat))
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

            fprintf(p_fp, "%03.6f,%s,%s,%d,%s\n", sec, p_XlatLogLevel(level), p_XlatLogCat(cat), line, buff);
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
        case LVL_DEBUG: strcpy(buff,  "DBG"); break;
        case LVL_ERROR: strcpy(buff,  "ERR"); break;
        default: snprintf(buff, sizeof(buff)-1, "???%d", level); break;
    }
    return buff;
}    

//--------------------------------------------------------//
const char* p_XlatLogCat(log_cat_t cat)
{
    static char buff[20];
    switch(cat)
    {
        case CAT_INIT:    strcpy(buff, "INIT"); break;
        case CAT_CREATE:  strcpy(buff, "CREATE"); break;
        case CAT_DESTROY: strcpy(buff, "DESTROY"); break;
        case CAT_ENTRY:   strcpy(buff, "ENTRY"); break;
        case CAT_EXIT:    strcpy(buff, "EXIT"); break;
        case CAT_LOOK:    strcpy(buff, "LOOK"); break;
        case CAT_SM:      strcpy(buff, "SM"); break;
        case CAT_MEM:     strcpy(buff, "MEM"); break;
        case CAT_USER:    strcpy(buff, "USER"); break;
        default: snprintf(buff, sizeof(buff)-1,  "???CAT_%d", cat); break;
    }
    return buff;
}    
