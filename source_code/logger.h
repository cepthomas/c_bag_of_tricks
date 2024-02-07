#ifndef LOGGER
#define LOGGER

#include <stdio.h>


//---------------- Public API ----------------------//

/// Log levels. See https://stackoverflow.com/a/2031209.
typedef enum
{
    LVL_NONE  = 0,
    LVL_TRACE = 1,
    LVL_DEBUG = 2,
    LVL_INFO  = 3,
    LVL_ERROR = 4
} log_level_t;


/// Initialize the module. If not called, logger_Log() is ignored.
/// @param fp File stream to write to. Can be stdout.
/// @return Status.
int logger_Init(FILE* fp);

/// Set log levels/cats. the module.
/// @param level
/// @return Status.
int logger_SetFilters(log_level_t level);

/// log some information.
/// @param level See log_level_t.
/// @param line Source line number.
/// @param format Format string followed by varargs.
/// @return Status.
int logger_Log(log_level_t level, int line, const char* format, ...);


/// Helper macros.
#define LOG_ERROR(fmt, ...)  logger_Log(LVL_ERROR, __LINE__, fmt, ##__VA_ARGS__);
#define LOG_INFO(fmt, ...)   logger_Log(LVL_INFO,  __LINE__, fmt, ##__VA_ARGS__);
#define LOG_DEBUG(fmt, ...)  logger_Log(LVL_DEBUG, __LINE__, fmt, ##__VA_ARGS__);
#define LOG_TRACE(fmt, ...)  logger_Log(LVL_TRACE, __LINE__, fmt, ##__VA_ARGS__);

#endif // LOGGER
