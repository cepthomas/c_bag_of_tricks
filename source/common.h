#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "logger.h"

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

/// Handler for alloc failures. Never returns.
/// @param line Number.
/// @param file Name.
void common_MemFail(int line, const char* file);

/// Validate pointer arg. If fails, early returns err.
/// @param ptr Pointer.
/// @param err Error value to return in case of failure.
#define VAL_PTR(ptr, err) if(ptr == NULL) { return err; }


//-------------------------- Managed lifetime -----------------------------//

/// A crude memory alloc/free probe mechanism. You can strip it out if you want.
#ifdef USE_PROBE
#define PROBE(mark, var, ln, fn) logger_Log(LVL_DEBUG, CAT_MEM, ln, "%s,%p,%d,\"%s\"", mark, var, ln, fn)
#else
#define PROBE(mark, var, ln, fn)
#endif

/// Make an instance of a typed thing. Client is responsible for FREE().
/// @param var Variable name.
/// @param type Thing type.
/// @param err Error value to return in case of failure.
#define CREATE_INST(var, type) \
    type* var = (type*)calloc(1, sizeof(type)); \
    PROBE("+++", var, __LINE__, __FILE__); \
    if(var == NULL) { common_MemFail(__LINE__, __FILE__); }

/// Make a standard char buff. Client is responsible for FREE().
/// @param var Variable name.
/// @param len String length. We add room for trailing 0.
#define CREATE_STR(var, len) \
    char* var = (char*)calloc(len + 1, sizeof(char)); \
    PROBE("+++", var, __LINE__, __FILE__); \
    if(var == NULL) { common_MemFail(__LINE__, __FILE__); }

/// Free the item.
/// @param var Variable name.
#define FREE(var) \
    PROBE("---", var, __LINE__, __FILE__); \
    free(var)

#endif // COMMON_H
