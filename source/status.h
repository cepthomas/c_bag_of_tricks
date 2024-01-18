#ifndef STATUS_H
#define STATUS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>


//---------------- Additional errnos -------------------//

/// Convenience for readability.
#define ENOERR            0
/// NULL argument. Could use EINVAL but this is a bit more specific.
#define EARGNULL        200
/// Index in the general/lua sense - could be an array index, collection key, list iterator. Also indicates empty.
#define EINVALIDINDEX   201


//---------------- Helpers ------------------------------//

/// Defines an invalid pointer (allocation, iniitialization, etc), typically that returned by a function.
/// This is distinct from NULL which may be a normal value e.g. end of an array.
#define BAD_PTR (void*)-1

/// Validate pointer. If fails, returns early with (typed) err. Typically used for function args.
#define VAL_PTR(ptr, err) { if(ptr == NULL) { logger_Log(LVL_ERROR, CAT_MEM, __LINE__, "VAL_PTR: %s", __FILE__); return err; } }

#endif // STATUS_H
