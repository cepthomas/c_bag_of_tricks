
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "common.h"
#include "logger.h"


//---------------- Private Declarations ------------------//

/// Logging support items.
static log_level_t p_level = LVL_INFO;
static log_cat_t p_cat = CAT_ALL;
static FILE* p_fp = NULL;

/// For timing.
static double p_start_time = 0;

/// Translate to human.
/// @param level to translate.
/// @return string version
static const char* p_XlatLogLevel(log_level_t level);

/// Translate to human.
/// @param cat to translate.
/// @return string version
static const char* p_XlatLogCat(log_cat_t cat);

/// Returns the current number of seconds since the epoch.
/// @return The sec.
static double p_GetCurrentSec();


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int logger_Init(FILE* fp)
{
    p_start_time = p_GetCurrentSec();
    VAL_PTR(fp, RS_ERR);
    p_fp = fp;
    return RS_PASS;
}

//--------------------------------------------------------//
int logger_SetFilters(log_level_t level, log_cat_t cat)
{
    assert(p_fp != NULL);

    p_level = level;
    p_cat = cat;
    return RS_PASS;
}

//--------------------------------------------------------//
int logger_Log(log_level_t level, log_cat_t cat, int line, const char* format, ...)
{
    assert(p_fp != NULL);

    VAL_PTR(p_fp, RS_ERR);
    VAL_PTR(format, RS_ERR);
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

        double current = p_GetCurrentSec();
        double sec = current - p_start_time;

        fprintf(p_fp, "%03.6f,%s,%s,%d,%s\n", sec, p_XlatLogLevel(level), p_XlatLogCat(cat), line, buff);
    }

    return RS_PASS;
}


//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
const char* p_XlatLogLevel(log_level_t level)
{
    static char buff[20];
    switch(level)
    {
        case LVL_INFO:  strcpy(buff, "INF"); break;
        case LVL_DEBUG: strcpy(buff, "DBG"); break;
        case LVL_ERROR: strcpy(buff, "!!!!!! ERR"); break;
        default: snprintf(buff, 20-1, "?????? LVL%d", level); break;
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
        case CAT_LOOK:    strcpy(buff, ">>>>>> LOOK"); break;
        case CAT_SM:      strcpy(buff, "SM"); break;
        case CAT_MEM:     strcpy(buff, "MEM"); break;
        case CAT_USER:    strcpy(buff, "USER"); break;
        default: snprintf(buff, 20-1, "?????? CAT_%d", cat); break;
    }
    return buff;
}    


//--------------------------------------------------------//
double p_GetCurrentSec()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    double sec = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    return sec;
}

