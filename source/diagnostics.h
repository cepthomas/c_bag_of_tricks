#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

// #include "logger.h" //TODO1 remove dependency - interface
// Cli command handler.
// typedef int (* const logger_t)(const cli_command_desc_t* pcmd, cli_args_t* args);


//-------------------------- Utilities --------------------------------//

/// Validate pointer arg. If fails, early returns err.
/// @param ptr Pointer.
/// @param err Error value to return in case of failure.
#define VAL_PTR(ptr, err) { if(ptr == NULL) { return err; } } // log __LINE__

/// Handler for alloc failures. Never returns - exits.
/// @param line Number.
/// @param file Name.
#define MEM_FAIL(line, file) { printf("Alloc/free failure: %s(%d)", file, line); exit(1); }
// #define MEM_FAIL(line, file) { logger_Log(LVL_ERROR, CAT_MEM, line, "Alloc/free failure: %s", file); exit(1); }


//-------------------------- Managed lifetime -----------------------------//

/// A crude memory alloc/free probe mechanism. Used to detect leaks during the collections development.
/// Enable it being changing the define below, building, then run proc_mem.py. You can strip it out if you want.
//#define PROBE(mark, var, ln, fn) logger_Log(LVL_DEBUG, CAT_MEM, __LINE__, "%s,%p,%d,\"%s\"", mark, var, __LINE__, fn)
#define PROBE(mark, var, ln, fn)

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