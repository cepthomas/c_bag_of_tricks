#ifndef STATUS_H
#define STATUS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include "status.h"


// For a hardened system (such as embedded), most real errors are considered unrecoverable.
// Things like handling comm timeouts should be considered normal behavior and handled accordingly.
// So errors are very bad and usually result in hard crash/reset. This of course should never
// happen because they have all been caught in unit and integration testing, right?

// ??? Rather than add a whole new error handling system, use existing C patterns:
// - Functions (usually low-level) that return pointers return BAD_PTR for alloc errors and NULL for normal op.
// - !!! don't do this: Functions that return things like counts (where 0 is valid) return RS_ERR (-1) for errors. ?????? what about valid negative numbers?
// - Functions that return status return RS_ERR (-1) for errors, RS_PASS (0) for success, RS_FAIL (-2) for failure (logical not internal).


// - common.h defines some macros:
//     - Return values for ints and pointers.
//     - Macros for creating and destroying typed objects with validation - CREATE_INST(), CREATE_STR(), FREE().
//     - Macros for validating arg pointers - VAL_PTR().
//     - Note that these macros use early returns to keep the if-nesting reasonable. Normally I disdain early returns but in this case the pluses outweigh.


//typedef signed int status_t;


int status_Encode (bool fail, bool logged, uint8_t subsystem, uint8_t category, uint8_t code);

//-------------------------- Status and return codes -----------------------------//


// byte 1: status - bit7 = error, bit6 = logged, etc...
// byte 2: subsystem - 0xF0 is reserved for cbot, 0x00 is reserved for convenient overlay with other code families.
// byte 3: category - app specific
// byte 4: code - app specific- When errors occur, set errno accordingly.
#define MAKE_STAT(fail, logged, subsystem, category, code) (fail ? 0x80000000 : 0x00000000) + (logged ? 0x40000000 : 0x00000000) + (subsystem << 16) + (category << 8) + (code)
// #define MAKE_STAT(fail, logged, subsystem, category, code) status_Encode(fail, logged, subsystem, category, code)

// byte 2: subsystem - 0xF0 is reserved for cbot, 0x00 is reserved for convenient overlay with other code families.
#define SUBSYS_CBOT 0xF0
#define SUBSYS_DEFAULT 0x00

/// Defines success for a function that returns int status.
#define CAT_PASS 0x00
/// Defines an unrecoverable error (memory, invalid data, etc) for a function that returns int status.
#define CAT_ERROR 0x01
/// Defines recoverable failure (expected, end of iteration, etc) for a function that returns int status.
#define CAT_FAIL 0x02



#define MAKE_STAT_PASS(subsystem, category, code) MAKE_STAT(false, false, subsystem, category, code)
#define MAKE_STAT_FAIL(subsystem, category, code) MAKE_STAT(true, false, subsystem, category, code)
// #define RS_PASS(rs) ((((rs) & RS_FAIL_BIT_MASK) == 0) ? true : false)
// #define RS_FAIL(rs) ((((rs) & RS_FAIL_BIT_MASK) == 0) ? false : true)
// #define RS_ERR_LOGGED(rs) ((((rs) & RS_LOGGED_BIT_MASK) == 0) ? false : true)
// #define RS_ERR_PENDING(rs) ((RS_FAIL(rs)) && (!(RS_ERR_LOGGED(rs))))
// #define RS_SET_FAIL(rs, fail) (rs = (((((int)fail << RS_FAIL_BIT_LOCATION)) & (RS_FAIL_BIT_MASK)) | (rs)))
// #define RS_SET_LOGGED(rs, logged) (rs = ((((int)logged << RS_LOGGED_BIT_LOCATION) & (RS_LOGGED_BIT_MASK)) | (rs)))


#define STAT_NO_ERR  0


#define CBOT_PASS MAKE_STAT_PASS(SUBSYS_CBOT, CAT_PASS, 0)

#define CBOT_ERR  MAKE_STAT_FAIL(SUBSYS_CBOT, CAT_ERROR, 1)

#define CBOT_FAIL MAKE_STAT_PASS(SUBSYS_CBOT, CAT_FAIL, 2)


#endif // STATUS_H
