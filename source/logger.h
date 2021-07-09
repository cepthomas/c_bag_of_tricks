#ifndef LOGGER
#define LOGGER

#include "common.h"


/// @brief Declaration of a logger module.

//---------------- Public API ----------------------//

/// Log levels. https://stackoverflow.com/a/2031209.
/// Trace - Only when I would be "tracing" the code and trying to find one part of a function specifically.
/// Debug - Information that is diagnostically helpful to people more than just developers (IT, sysadmins, etc.).
/// Info - Generally useful information to log (service start/stop, configuration assumptions, etc). Info I want to always have
///    available but usually don't care about under normal circumstances. This is my out-of-the-box config level.
/// Warn - Anything that can potentially cause application oddities, but for which I am automatically recovering. (Such as switching
///    from a primary to backup server, retrying an operation, missing secondary data, etc.)
/// Error - Any error which is fatal to the operation, but not the service or application (can't open a required file, missing data, etc.).
///    These errors will force user (administrator, or direct user) intervention. These are usually reserved (in my apps) for incorrect
///    connection strings, missing services, etc.
/// Fatal - Any error that is forcing a shutdown of the service or application to prevent data loss (or further data loss). I reserve these only
///    for the most heinous errors and situations where there is guaranteed to have been data corruption or loss.
typedef enum
{
    LVL_DEBUG = 1 << 1,
    LVL_INFO  = 1 << 2,
    LVL_ERROR = 1 << 4,
    LVL_ALL   = 0xFFFF
} log_level_t;



/// Initialize the module.
/// @param fp File to write to or stdout if NULL.
/// @return Status.
int logger_Init(FILE* fp);

/// Set log levels/cats. the module.
/// @param level
/// @param cat
/// @return Status.
int logger_SetFilters(log_level_t level, int cat);

/// Shutdown the module.
/// @return Status.
int logger_Destroy(void);

/// log some information.
/// @param level See log_level_t.
/// @param cat User supplied.
/// @param line Source line number.
/// @param format Format string followed by varargs.
/// @return Status.
int logger_Log(log_level_t level, int cat, int line, const char* format, ...);


/// Helper macros.
#define LOG_ERROR(cats, fmt, ...)  logger_Log(LVL_ERROR, cats, __LINE__, fmt, __VA_ARGS__);
#define LOG_INFO(cats, fmt, ...)   logger_Log(LVL_INFO, cats, __LINE__, fmt, __VA_ARGS__);
#define LOG_DEBUG(cats, fmt, ...)  logger_Log(LVL_DEBUG, cats, __LINE__, fmt, __VA_ARGS__);

#endif // LOGGER
