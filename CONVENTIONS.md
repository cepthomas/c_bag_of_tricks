
# Conventions TODO2 rework/reorg/relocate
- All C projects use this naming convention. It's basically the
  [Barr Group standard](https://barrgroup.com/embedded-systems/books/embedded-c-coding-standard) with some minor adjustments, because.
- Uses the model described in [c_modular](https://github.com/cepthomas/c_modular).

## General Naming
  - variables are `int snake_case`.
  - private variables are `static int p_snake_case`.
  - constants and defines are `UPPER_CASE`.
  - public functions are `module_MyFunc()`.
  - private functions are `p_MyFunc()`.
  - `int* pint`, not `int *pint`.

## ============= How to build/run/test
- build.cmd - app
- run.cmd - app
- test.cmd - unit



## ============= Error Handling
For a hardened system (such as embedded), most real errors are considered unrecoverable. Things like handling comm timeouts should be considered
normal behavior and handled accordingly. So errors are very bad and usually result in hard crash/reset. This of course should never
happen because they have all been caught in unit and integration testing, right?

Rather than add a whole new error handling system, use existing C patterns:
- Functions that return pointers return BAD_PTR (NULL) for errors.
- Functions that return things like counts (where 0 is valid) return RS_ERR (-1) for errors. ?????? what about valid negative numbers?
- Functions that return status return RS_ERR (-1) for errors, RS_PASS (0) for success, RS_FAIL (-2) for failure (logical not internal).
- When errors occur, set errno accordingly.
- common.h defines some macros:
    - Return values for ints and pointers.
    - Macros for creating and destroying typed objects with validation - CREATE_INST(), CREATE_STR(), FREE().
    - Macros for validating arg pointers - VAL_PTR().
    - Note that these macros use early returns to keep the if-nesting reasonable. Normally I disdain early returns but in this case the pluses outweigh.


## Documenting The Code
``` C

/// Define a useful macro.
/// @param stat blabla.
/// @param func blabla.
/// @return blabla.
#define MY_MACRO(stat, func, ...) \
{ \
    stat = func(__VA_ARGS__); \
}

/// Define a function pointer type.
/// @param foo blabla.
/// @return Something.
typedef int (*modulex_InterruptFunc_t)(int foo);

/// Define a discrete value type. Preferred method.
typedef enum
{
    SOME_OK = 0,  ///> I'm ok
    SOME_ERROR,   ///> Not so good.
} some_t;

/// Define a discrete value type. Second best method.
const int SOME_OK = 0;

/// Define a discrete value type. Only if necessary.
#define SOME_OK 0

/// Initialize the module.
/// @param fp blabla.
/// @return Status.
int modulex_Init(modulex_InterruptFunc_t fp);

/// Frees all data pointers, and the string struct.
/// @return T/F.
bool modulex_Destroy(void);

/// Private declarations look like this.
int p_some_value = 0;

/// Function blabla.
/// @param index blabla.
/// @return some value..
long p_SomeUtility(int index);
```
