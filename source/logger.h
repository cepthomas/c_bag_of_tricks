#ifndef LOGGER
#define LOGGER

#include <stdio.h>

#include "common.h"


/// @brief Declaration of a logger module.

//---------------- Public API ----------------------//

/// Log levels. See https://stackoverflow.com/a/2031209.
typedef enum
{
    LVL_NONE  = 0,
    LVL_DEBUG = 1,
    LVL_INFO  = 2,
    LVL_ERROR = 3
} log_level_t;

/// Log categories. Add more as needed - and update p_XlatLogCat(). Make client configurable?
typedef enum
{
    CAT_INIT     = 1 << 0,
    CAT_CREATE   = 1 << 1,
    CAT_DESTROY  = 1 << 2,
    CAT_ENTRY    = 1 << 3,
    CAT_EXIT     = 1 << 4,
    CAT_LOOK     = 1 << 5,
    CAT_SM       = 1 << 6,
    CAT_MEM      = 1 << 7,
    CAT_USER     = 1 << 15,
    CAT_ALL      = 0xFFFFFFFF
} log_cat_t;


/// Initialize the module.
/// @param fp File to write to or stdout if NULL.
/// @return Status.
int logger_Init(FILE* fp);

/// Set log levels/cats. the module.
/// @param level
/// @param cat
/// @return Status.
int logger_SetFilters(log_level_t level, log_cat_t cat);

/// Shutdown the module.
/// @return Status.
int logger_Destroy(void);

/// log some information.
/// @param level See log_level_t.
/// @param cat User supplied.
/// @param line Source line number.
/// @param format Format string followed by varargs.
/// @return Status.
int logger_Log(log_level_t level, log_cat_t cat, int line, const char* format, ...);


/// Helper macros.
#define LOG_ERROR(cat, fmt, ...)  logger_Log(LVL_ERROR, cat, __LINE__, fmt, ##__VA_ARGS__);
#define LOG_INFO(cat, fmt, ...)   logger_Log(LVL_INFO,  cat, __LINE__, fmt, ##__VA_ARGS__);
#define LOG_DEBUG(cat, fmt, ...)  logger_Log(LVL_DEBUG, cat, __LINE__, fmt, ##__VA_ARGS__);

#endif // LOGGER
