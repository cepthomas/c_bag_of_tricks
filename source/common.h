#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

/// Make an instance of a thing, set to 0.
/// @param V Var name.
/// @param T The thing type.
/// @return Typed pointer to the new thing.
#define CREATE_INST(V, T) T* V = (T*)calloc(1, sizeof(T))

/// Make a standard char buff.
/// @param V Var name.
/// @param N Var length. We add room for trailing 0.
/// @return char*.
#define CREATE_STR(V, N) char* V = (char*)calloc(N + 1, sizeof(char))

/// Delete one of the things created above.
/// @param V Var.
#define DESTROY(V) free(V)

#endif // COMMON_H