#ifndef STATUS_H
#define STATUS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>


// For a hardened system (such as embedded), most real errors are considered unrecoverable.
// Things like handling comm timeouts should be considered normal behavior and handled accordingly.
// So errors are very bad and usually result in hard crash/reset. This of course should never
// happen because they have all been caught in unit and integration testing, right?

// Rather than add a whole new error handling system, (ab)use existing C patterns:
// - Most functions return an int status code consisting of standard errno values, plus
//   additional cbot specific values starting from 200.
// - Functions that return things like counts/length/index - for errors will return an errno plus the MSB set.
//   This essentially makes them a negative for easy (< 0) testing by the caller. See MAKE_FAIL_ERRNO().
// - Functions (usually low-level) that return pointers return BAD_PTR for hard alloc errors and
//   NULL for normal operations such as indicating end of iteration.

#define ENOERR         0 // convenience
#define EARGNULL     200 // null argument
#define EEMPTY       201 // indicates empty container
#define EINVALIDKEY  202 // item not in collection
#define EITEREND     203 // invalid iterator.

// Tacks MSB=1 on to errno. 
#define MAKE_FAIL_ERRNO(errno) ((errno & 0x000000FF) | 0x80000000)

/// Defines an error (allocation, iniitialization, etc) for a function that returns a pointer.
/// This is distinct from NULL which may be a normal value e.g. end of iteration.
#define BAD_PTR (void*)-1


#endif // STATUS_H
