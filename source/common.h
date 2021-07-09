#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>


//-------------------------- Return codes -----------------------------//

/// Defines an error (allocation, iniitialization, etc) for a function that returns a pointer.
#define BAD_PTR NULL

/// Defines success for a function that returns int status.
static const int RS_PASS = 0;

/// Defines an error (memory, invalid data, etc) for a function that returns int status.
static const int RS_ERR = -1;

/// Defines failure (expected, end of iteration, etc) for a function that returns int status.
static const int RS_FAIL = -2;


//-------------------------- Utilities --------------------------------//

/// Startup.
/// @return Status.
int common_Init();

/// Returns the number of milliseconds since the app was started.
/// @return The msec.
double common_GetElapsedSec(void);

/// Returns the current number of seconds since the epoch.
/// @return The sec.
double common_GetCurrentSec(void);

/// Helper macro.
// ... CHECKED_FUNC(stat, hal_RegTimerInterrupt, SYS_TICK_MSEC, p_TimerHandler);
#define CHECKED_FUNC(stat, func, ...) \
{ \
    stat = func(__VA_ARGS__); \
    if(stat != STATUS_OK) \
    { \
        common_log(LVL_ERROR, #func); \
    } \
}


//-------------------------- Managed lifetime -----------------------------//

/// A crude memory alloc/free probe mechanism. You can strip it out if you want. TODOP
#ifdef USE_PROBE
#define PROBE(mark, var, ln, fn) printf("%s,%p,%d,\"%s\"\n", mark, var, ln, fn)
#else
#define PROBE(mark, var, ln, fn)
#endif

/// Make an instance of a typed thing with all bytes set to 0. Client is responsible for FREE().
/// @param var Thing variable name.
/// @param type Thing type.
/// @param err Error value to return in case of failure.
#define CREATE_INST(var, type, err) \
    type* var = (type*)calloc(1, sizeof(type)); \
    PROBE("+++", var, __LINE__, __FILE__); \
    if(var == NULL) { errno = ENOMEM; return err; }

/// Make a standard char buff. Client is responsible for FREE().
/// @param var String variable name.
/// @param len String length. We add room for trailing 0.
/// @param err Error value to return in case of failure.
#define CREATE_STR(var, len, err) \
    char* var = (char*)calloc(len + 1, sizeof(char)); \
    PROBE("+++", var, __LINE__, __FILE__); \
    if(var == NULL) { errno = ENOMEM; return err; }

/// Free the item.
/// @param var String variable name.
#define FREE(var) \
    PROBE("---", var, __LINE__, __FILE__); \
    free(var)

/// Validate pointer arg. If fails, sets errno and early returns.
/// @param ptr Pointer.
/// @param err Error value to return in case of failure.
#define VAL_PTR(ptr, err) \
    if(ptr == NULL) { errno = EINVAL; return err; }

#endif // COMMON_H
