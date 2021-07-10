
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "logger.h"


/// @brief Definition of logger.


//---------------- Private Declarations ------------------//

/// Logging support items.
log_level_t p_level = LVL_INFO;
log_cat_t p_cat = CAT_ALL;
FILE* p_fp = NULL;


/// Translate to human.
/// @param level to translate.
/// @return string version
const char* p_XlatLogLevel(log_level_t level);

/// Translate to human.
/// @param cat to translate.
/// @return string version
const char* p_XlatLogCat(log_cat_t cat);


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int logger_Init(FILE* fp)
{
    p_fp = fp == NULL ? stdout : fp;
    return 0;
}

//--------------------------------------------------------//
int logger_SetFilters(log_level_t level, log_cat_t cat)
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
int logger_Log(log_level_t level, log_cat_t cat, int line, const char* format, ...)
{
    VAL_PTR(format, RS_ERR);
    #define LOG_LINE_LEN 100
    static char buff[LOG_LINE_LEN];

    if(p_fp == NULL)
    {
        logger_Init(NULL);
        logger_Log(LVL_ERROR, cat, line, "You forgot to call logger_Init() so I did it for you.");
    }

    // Check filters.
    if((level >= p_level) && (cat & p_cat))
    {
        va_list args;
        va_start(args, format);
        // The content.
        vsnprintf(buff, LOG_LINE_LEN-1, format, args);
        va_end(args);

        fprintf(p_fp, "%03.6f,%s,%s,%d,%s\n", common_GetElapsedSec(), p_XlatLogLevel(level), p_XlatLogCat(cat), line, buff);
        // fprintf(p_fp, "%03.6f %s %s (%d) %s\n", common_GetElapsedSec(), p_XlatLogLevel(level), p_XlatLogCat(cat), line, buff);
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
