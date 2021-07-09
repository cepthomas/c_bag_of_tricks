
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "logger.h"


/// @brief Definition of logger.


//---------------- Private Declarations ------------------//

log_level_t p_level = LVL_INFO;
int p_cat = 0xFFFFFFFF;
FILE* p_fp = NULL;

#define LOG_LINE_LEN 100
//static const int LOG_LINE_LEN = 100;
static char p_log_buff[LOG_LINE_LEN];


const char* p_XlatLogLevel(log_level_t level);

const char* p_XlatLogCat(int cat);


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int logger_Init(FILE* fp)
{
    p_fp = fp == NULL ? stdout : fp;
    return 0;
}

//--------------------------------------------------------//
int logger_SetFilters(log_level_t level, int cat)
{
    p_level = level;
    p_cat = cat;
    return 0;
}

//--------------------------------------------------------//
int logger_Destroy(void)
{
    // Anything?
    return 0;
}

//--------------------------------------------------------//
int logger_Log(log_level_t level, int cat, int line, const char* format, ...)
{
    VAL_PTR(format, RS_ERR);

    // Check filters.
    if((level & p_level) && (cat & p_cat))
    {
        va_list args;
        va_start(args, format);
        // The content.
        vsnprintf(p_log_buff, LOG_LINE_LEN-1, format, args);
        va_end(args);

        fprintf(p_fp, "%03.6f %s %s (%d) %s\n", common_GetElapsedSec(), p_XlatLogLevel(level), p_XlatLogCat(cat), line, p_log_buff);

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
        case LVL_ERROR: strcpy(buff, "ERR"); break;
        default: snprintf(buff, 20-1, "%d", level); break;
    }
    return buff;
}    

//--------------------------------------------------------//
const char* p_XlatLogCat(int cat) //TODO from user
{
    static char buff[20];
    switch(cat)
    {
        // case LOG_INFO:  strcpy(buff, "INF"); break;
        // case LOG_WARN:  strcpy(buff, "WRN"); break;
        // case LOG_ERROR: strcpy(buff, "ERR"); break;
        default: snprintf(buff, 20, "CAT%d", cat); break;
    }
    return buff;
}    

