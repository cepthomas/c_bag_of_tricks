#ifndef STATUS_H
#define STATUS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>


//---------------- Additional errno ----------------------//

/// Convenience for readability.
#define ENOERR            0

/// NULL argument. Could use EINVAL but this is a bit more specific.
#define EARGNULL        200

/// Index in the general/lua sense - could be an array index, collection key, list iterator. Also indicates empty access.
#define EINVALIDINDEX   201

/// Defines an error (allocation, iniitialization, etc) for a function that returns a pointer.
/// TODO1 also references bad ptr - needs better name + logging/printf.
/// This is distinct from NULL which may be a normal value e.g. end of iteration.
#define BAD_PTR (void*)-1

#endif // STATUS_H
