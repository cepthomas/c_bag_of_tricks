#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include "logger.h"


//-------------------------- Managed lifetime -----------------------------//

/// A crude memory alloc/free probe mechanism. Used to detect leaks during the collections development.
/// Enable it being changing the define below, building, then run proc_mem.py. You can strip it out if you want.
#ifdef USE_PROBE
#define PROBE(mark, var, ln, fn) LOG_TRACE("%s,%p,\"%s\"", mark, var, fn)
#else
#define PROBE(mark, var, ln, fn)
#endif

/// Handler for alloc failures. Never returns - exits.
/// @param line Number.
/// @param file Name.
#ifdef USE_PROBE
#define MEM_FAIL(line, file) { logger_Log(LVL_ERROR, line, "MEM_FAIL: %s", file); exit(1); }
#else
#define MEM_FAIL(line, file) exit(1)
#endif

// Common part.
#define _CREATE(var) \
    PROBE("+++", var, __LINE__, __FILE__); \
    if(var == NULL) { MEM_FAIL(__LINE__, __FILE__); };

/// Make an instance of a typed thing. Client is responsible for FREE().
/// @param var Variable name.
/// @param type Variable type.
#define CREATE_INST(var, type) \
    type* var = (type*)calloc(1, sizeof(type)); \
    _CREATE(var);

/// Make a standard char buff. Client is responsible for FREE().
/// @param var Variable name.
/// @param len String length. We add room for trailing 0.
#define CREATE_STR(var, len) \
    char* var = (char*)calloc(len + 1, sizeof(char));  \
    _CREATE(var);

/// Free the item.
/// @param var Variable name.
#define FREE(var) \
    if(var == NULL) { MEM_FAIL(__LINE__, __FILE__); } \
    PROBE("---", var, __LINE__, __FILE__); \
    free(var);

#endif // COMMON_H
