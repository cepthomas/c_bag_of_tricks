#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

//-------------------------- Typed lifetime -----------------------------//

/// Make an instance of a thing, set to 0.
/// @param V Thing variable name.
/// @param T The thing type.
/// @return Typed pointer to the new thing.
#define CREATE_INST(V, T) T* V = (T*)calloc(1, sizeof(T))

/// Make a standard char buff.
/// @param V String variable name.
/// @param N String length. We add room for trailing 0.
/// @return char*.
#define CREATE_STR(V, N) char* V = (char*)calloc(N + 1, sizeof(char))

/// Delete one of the things created above.
/// @param V Thing variable name.
#define DESTROY(V) free(V)


//-------------------------- Error handling -----------------------------//

/// Defines an error (allocation, iniitialization, etc) for a function that returns a pointer.
#define PTR_ERR NULL

/// Defines an error (memory, invalid data, etc) for a function that returns a status int.
#define RET_ERR -1

/// Defines success for a function that returns a status int.
#define RET_PASS 0

/// Defines failure (not an error - invalid index, etc) for a function that returns a status int.
#define RET_FAIL 1

#endif // COMMON_H
