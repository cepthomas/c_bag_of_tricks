#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

// TODO pretty these up.
#define FREE(ptr) printf("---,%p,%d,\"%s\"\n", ptr, __LINE__, __FILE__); free(ptr)

//-------------------------- Typed lifetime -----------------------------//

/// Make an instance of a typed thing with all bytes set to 0. Client is responsible for free().
/// @param var Thing variable name.
/// @param type The thing type.
/// @param err Error value to return in case of failure.
#define CREATE_INST(var, type, err) type* var = (type*)calloc(1, sizeof(type)); printf("+++,%p,%d,\"%s\"\n", var, __LINE__, __FILE__); if(var == NULL) { errno = ENOMEM; return err; }

/// Make a standard char buff. Client is responsible for free().
/// @param var String variable name.
/// @param len String length. We add room for trailing 0.
/// @param err Error value to return in case of failure.
#define CREATE_STR(var, len, err) char* var = (char*)calloc(len + 1, sizeof(char)); printf("+++,%p,%d,\"%s\"\n", var, __LINE__, __FILE__); if(var == NULL) { errno = ENOMEM; return err; }


//-------------------------- Error handling -----------------------------//

/// Defines an error (allocation, iniitialization, etc) for a function that returns a pointer.
#define BAD_PTR NULL

/// Defines an error (memory, invalid data, etc) for a function that returns status int.
#define RS_ERR -1

/// Defines success for a function that returns status int.
#define RS_PASS 0

/// Defines failure (expected, end of iteration, etc) for a function that returns status int.
#define RS_FAIL -2

/// Validate pointer arg. If fails, sets errno and early returns.
/// @param ptr Pointer.
/// @param err Error value to return in case of failure.
#define VAL_PTR(ptr, err) if(ptr == NULL) { errno = EINVAL; return err; }

// /// Validate pointer arg. If fails, sets errno and early returns BAD_PTR.
// /// @param ptr Pointer.
// #define VALPTR_PTR(ptr) if(ptr == NULL) { errno = EINVAL; return BAD_PTR; }

// /// Validate pointer arg. If fails, sets errno and early returns RS_ERR.
// /// @param ptr Pointer.
// #define VALPTR_RS(ptr) if(ptr == NULL) { errno = EINVAL; return RS_ERR; }

#endif // COMMON_H
