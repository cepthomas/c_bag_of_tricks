#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

//-------------------------- Typed lifetime -----------------------------//

/// Make an instance of a typed thing with all bytes set to 0. Client is responsible for free().
/// @param V Thing variable name.
/// @param T The thing type.
/// @param E Error value to return in case of failure.
#define CREATE_INST(V, T) T* V = (T*)calloc(1, sizeof(T))

/// Make a standard char buff. Client is responsible for free().
/// @param V String variable name.
/// @param N String length. We add room for trailing 0.
#define CREATE_STR(V, N) char* V = (char*)calloc(N + 1, sizeof(char))


//-------------------------- Error handling -----------------------------//

/// Defines an error (allocation, iniitialization, etc) for a function that returns a pointer.
#define BAD_PTR NULL

/// Defines an error (memory, invalid data, etc) for a function that returns status int.
#define RS_ERR -1

/// Defines success for a function that returns status int.
#define RS_PASS 0

/// Defines failure (expected, end of iteration, etc) for a function that returns status int.
#define RS_FAIL -2

/// Validate pointer. If fails, sets errno and early returns BAD_PTR.
/// @param ptr Pointer.
#define VALPTR_PTR(ptr) if(ptr == NULL) { errno = EINVAL; return BAD_PTR; }

/// Validate pointer. If fails, sets errno and early returns RS_ERR.
/// @param ptr Pointer.
#define VALPTR_RS(ptr) if(ptr == NULL) { errno = EINVAL; return RS_ERR; }

#endif // COMMON_H
