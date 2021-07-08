#ifndef LOGGER
#define LOGGER

#include <common.h>

/// @brief Declaration of a module named thing.

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
    LOG_TRACE = 1 << 0,
    LOG_DEBUG = 1 << 1,
    LOG_INFO  = 1 << 2,
    LOG_WARN  = 1 << 3,
    LOG_ERROR = 1 << 4,
    LOG_FATAL = 1 << 5,
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
/// @param cats User supplied.
/// @param line Source line number.
/// @param format Format string followed by varargs.
void logger_Log(log_level_t level, int cats, int line, const char* format, ...);


#define eeeprintf(...) fprintf (stderr, __VA_ARGS__);

/// Helper macros.
#define LOG_ERR(cats, fmt, ...)   logger_Log(log_level_t.LOG_ERR, cats, __LINE__, fmt, __VA_ARGS__);
#define LOG_INFO(cats, fmt, ...)   logger_Log(log_level_t.LOG_INFO, cats, __LINE__, fmt, __VA_ARGS__);
#define LOG_DEBUG(cats, fmt, ...)   logger_Log(log_level_t.LOG_DEBUG, cats, __LINE__, fmt, __VA_ARGS__);

#endif // LOGGER
