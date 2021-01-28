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
#define RET_PTR_ERR NULL

/// Defines an error (memory, invalid data, etc) for a function that returns a status int.
#define RET_ERR -1

/// Defines success for a function that returns a status int.
#define RET_PASS 0

/// Defines failure (expected, end of iteration, etc) for a function that returns a status int.
#define RET_FAIL -2

/// Validate function pointer arguments. If fails, sets errno and early returns error.
/// @param ptr Pointer.
/// @param eret What to return if error.
#define VALIDATE_PTR1(ptr, eret) if(ptr == NULL) { errno = EINVAL; return eret; }

/// Validate function pointer arguments. If fails, sets errno and early returns error.
/// @param ptr1 Pointer.
/// @param ptr2 Pointer.
/// @param eret What to return if error.
#define VALIDATE_PTR2(ptr1, ptr2, eret) if(ptr1 == NULL || ptr2 == NULL) { errno = EINVAL; return eret; }

#endif // COMMON_H
