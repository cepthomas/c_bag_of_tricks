#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

//-------------------------- Typed lifetime -----------------------------//

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

/// Does this confuse the parser?


//-------------------------- Error handling -----------------------------//

// /// General status info.
// typedef enum severity
// {
//     SEV_OK = 0, ///< Default.
//     SEV_WARN,   ///< Something.
//     SEV_ERR,    ///< Recoverable.
//     SEV_FATAL   ///< Not recoverable.
// } severity_t;

// functions return:
//  - pointer or NULL
//  - uint >=0 or negative
//  - int like  long strtol(const char *restrict str, char **restrict endptr, int base);
//      If the correct value is outside the range of representable values,
//      {LONG_MIN}, {LONG_MAX}, {LLONG_MIN}, or {LLONG_MAX} shall be returned (according to the sign of the value), and errno set to [ERANGE].


// // Bins for category. 16 bit? Or is simple good/bad adequate?
// #define WARN_FLAG  0X4000
// #define FATAL_FLAG 0X8000

// /// General status info.
// typedef enum scode
// {
//     SC_OK = 0,  ///< Default.
//     // Recoverable.
//     SC_R11    ///< This one.
//     SC_R22,     ///< Or this one.
//     // Maybe recoverable.\?
//     SC_WREC = WARN_FLAG,
//     SC_W55,     ///< This one.
//     SC_W66,     ///< Or this one.
//     // Not recoverable stuff.
//     SC_NREC = FATAL_FLAG,
//     SC_F88,     ///< Or this.
//     SC_F99      ///< Or this.
// } scode_t;


// We will use return codes compatible with the std C libs:
// The value in errno is significant only when the return value of the call indicated an error (i.e., -1 from most system calls; -1 or 
// NULL from most library functions); a function that succeeds is allowed to change errno.  The value of errno is never set to 
// zero by any system call or library function.
// For some system calls and library functions (e.g., getpriority(2)), -1 is a valid return on success.  In such cases, a successful 
// return can be distinguished from an error return by setting errno to zero before the call, and then, if the call returns a status 
// that indicates that an error may have occurred, checking to see if errno has a nonzero value.
// errno is defined by the ISO C standard to be a modifiable lvalue of type int, and must not be explicitly declared; errno may be a macro.
// errno is thread-local; setting it in one thread does not affect its value in any other thread.

/// Defines an error (allocation, iniitialization, etc) for a function that returns a pointer.
#define PTR_ERR NULL

/// Defines an error (memory, invalid data, etc) for a function that returns a status int.
#define RET_ERR -1

/// Defines success for a function that returns a status int.
#define RET_PASS 0

/// Defines failure (not an error - invalid index, etc) for a function that returns a status int.
#define RET_FAIL 1

// These are some system values we may use:
// #define EPERM        1  /* Operation not permitted */
// #define EIO      5  /* Input/output error */
// #define ENXIO        6  /* No such device or address */
// #define E2BIG        7  /* Arg list too long */
// #define EAGAIN      11  /* Resource temporarily unavailable */
// #define ENOMEM      12  /* Not enough space */
// #define EACCES      13  /* Permission denied */
// #define EFAULT      14  /* Bad address */
// #define EBUSY       16   Device or resource busy 
// #define ENODEV      19  /* No such device */
// #define EINVAL      22  /* Invalid argument */
// #define ENOTTY      25  /* Inappropriate I/O control operation */
// #define ENOSPC      28  /* No space left on device */
// #define ESPIPE      29  /* Invalid seek (seek on a pipe?) */
// #define EDOM        33  /* Domain error (math functions) */
// #define ERANGE      34  /* Result too large (possibly too small) */
// #define EILSEQ      42  /* Illegal byte sequence */







/// Macro to minimize boilerplate. Does this confuse too?
// #define CHECKED_FUNC(_stat, _func, ...)
// {
//     _stat = _func(__VA_ARGS__);
//     if(_stat != STATUS_OK)
//     {
//         common_log(0, "%s(%d) %s", __FILE__, __LINE__, #_func);
//     }
// }
// use >>>>>>>>>>>
// CHECKED_FUNC(stat, hal_regTimerInterrupt, SYS_TICK_MSEC, p_timerHandler);


// Expected/valid returns:
// void* - NOT_NULL
// void VOID
// int GT0
// int GTE0
// RET_STAT_PASS

/// sssssss
/// @param _rs The retstat.
/// @param _int return value.
/// @param _func To execute.
/// @param ... var args
#define EXPECT_INT_GTE0(_rs, _int, _func, ...) \
{ \
    if(_rs < FATAL_FLAG) \
    { \
        _int = _func(__VA_ARGS__); \
        if(_int < 0) \
        { \
            _rs.code = SC_DEF; \
        } \
    } \
}


/// Type for application functions return. Default values (0/NULL) are considered valid.
// typedef struct retstat
// {
//     //severity_t severity;  ///< Specific.
//     scode_t code;           ///< Specific.
//     //void* value;          ///< Return value?
//     char* info;             ///< Could be anything.
//     int extra;              ///< Could be line number or code.
// } retstat_t;

// #define CREATE_RS(V) retstat_t V = { .code = SC_OK, .info = NULL, .extra = 0 }


#endif // COMMON_H


/*
-------------------- error handling ------------------------------
If the only possible errors are programmer errors, don't return an error code, use asserts inside the function.

/// Macros to avoid having to type this repeatedly.
#define _RS if(RET_STAT_PASS(RetStat)) RetStat =
#define _RC if(RET_STAT_PASS(RetStat))
#define _RF if(RET_STAT_FAIL(RetStat))
#define _RL if(!RET_STAT_ERR_LOGGED(RetStat))

/// Macro to minimize boilerplate.
#define CHECKED_FUNC(_stat, _func, ...) \
{ \
    _stat = _func(__VA_ARGS__); \
    if(_stat != STATUS_OK) \
    { \
        common_log(0, "%s(%d) %s", __FILE__, __LINE__, #_func); \
    } \
}
// >>>>>>>>>>>
CHECKED_FUNC(stat, hal_regTimerInterrupt, SYS_TICK_MSEC, p_timerHandler);



The malloc() function allocates size bytes and returns a pointer to the allocated memory. The memory is not initialized. If size is 0, then malloc() returns either NULL, or a unique pointer value that can later be successfully passed to free().

The free() function frees the memory space pointed to by ptr, which must have been returned by a previous call to malloc(), calloc() or realloc(). Otherwise, or if free(ptr) has already been called before, undefined behavior occurs. If ptr is NULL, no operation is performed.

typedef struct {
    enum {SUCCESS, ERROR} status; SEVERITY  ORIGIN
    union {
        int errCode;
        MyType value;
    } ret;
} MyTypeWrapper;

struct ErrorInfo
{
    int errorCode;
    char *errorMessage;
#if DEBUG
    char *functionName;
    int lineNumber;
#endif
    ErrorInfo *pInnerError;    // Pointer to previous error that may have led to this one
}

FILE *OpenFile(char *filename, ErrorInfo *pErrorInfo)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return(ChainedErrorNull(pErrorInfo, "Couldn't open file"));

    return(fp);
}


*/
