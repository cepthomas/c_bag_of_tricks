#ifndef CBOT_H
#define CBOT_H

#include <string.h>
#include "logger.h"


//---------------- Error codes -------------------//

/// No error.
#define CBOT_ERR_NO_ERR            0

/// Invalid argument. EINVAL 22
#define CBOT_ERR_INVALID_ARG     100

/// NULL argument. Could use CBOT_ERR_INVALID_ARG but this is a bit more specific.
#define CBOT_ERR_ARG_NULL        101

/// No data available. ENODATA 61
#define CBOT_ERR_NO_DATA         102

/// Index in the general/lua sense - could be an array index, collection key, list iterator. Also indicates empty.
#define CBOT_ERR_INVALID_INDEX   103

/// Defines an invalid pointer (allocation, iniitialization, etc), typically that returned by a function.
#define BAD_PTR     ((void*)-1)

/// Convenience to bookend BAD_PTR. Distinct because it may be a normal value e.g. end of a list.
#define NULL_PTR    ((void*)NULL)

#endif // CBOT_H
