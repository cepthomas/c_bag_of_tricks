
General naming.
  - variables are `int snake_case`.
  - private variables are `static int p_snake_case`.
  - constants and defines are `UPPER_CASE`.
  - public functions are `module_MyFunc()`.
  - private functions are `p_MyFunc()`.
  - `int* pint`, not `int *pint`.

Header files specify the public facing API.
``` C
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stuff.h>

/// @brief Declaration of a module named thing.

//---------------- Public API ----------------------//

/// Define a useful macro.
/// @param[in] stat blabla.
/// @param[in] func blabla.
/// @return blabla.
#define CHECKED_FUNC(stat, func, ...) \
{ \
    stat = func(__VA_ARGS__); \
}

/// Define a function pointer type.
/// @param[in] foo blabla.
/// @return Something.
typedef int (*thing_InterruptFunc_t)(int foo);

/// Define a discrete value type. Preferred method.
typedef enum
{
    STATUS_OK = 0,  ///> I'm ok
    STATUS_ERROR,   ///> Not so good.
} status_t;

/// Define a discrete value type. Second best method.
const int STATUS_OK = 0;

/// Define a discrete value type. Only if necessary.
#define STATUS_OK 0

/// Initialize the module.
/// @param[in] fp blabla.
/// @return Status.
status_t thing_Init(thing_InterruptFunc_t fp);

/// Frees all data pointers, and the string struct.
/// @return T/F.
bool thing_Destroy(void);

#endif // TEMPLATE_H
```

Code files implement the functionality.
``` C
#include "template.h"

//---------------- Private Declarations ------------------//

/// What is.
int p_some_value = 0;


/// Function blabla.
/// @param[in] index blabla.
/// @return some value..
long p_SomeUtility(int index);

//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
status_t thing_Init(thing_InterruptFunc_t fp)
{
    // Do something.
}

//--------------------------------------------------------//
bool thing_Destroy(void)
{
    // Do something.
}

//---------------- Private Implementation ----------------//

//--------------------------------------------------------//
long p_SomeUtility(int index)
{
    // Do something.
}
```
