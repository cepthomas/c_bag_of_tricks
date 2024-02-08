# C Coding Conventions

It's basically the [Barr Group standard](https://barrgroup.com/embedded-systems/books/embedded-c-coding-standard)
with some minor adjustments, because.

It's also useful to refer to [C module model](https://github.com/cepthomas/c_modular/blob/master/README.md) for
file and directory naming.

The goal is pure C99 which should compile anywhere. Sometimes test executables need to be platform-specific.

## Naming
- variables are `int snake_case`.
- private variables are `static int p_snake_case`.
- constants and defines are `UPPER_CASE`.
- public functions are `return_type module_MyFunc(args...)`.
- private functions are `return_type p_MyFunc(args...)` and private vars are `static bool p_my_status`.
  A reasonable variation is to use `_` instead of `p_`.
- `int* pint`, not `int *pint` because the type is a pointer not an int.
- Code documentation uses the `///` variation of javadoc, suitable for doxygen.  


## Status and Return Codes
- Functions return an int status code of 0 for OK or in the range of 100 to 127. This allows application-specific
  codes to overlap errno if need be.
- Some functions like count/length/index return valid numerical values which are always `>= 0`.
  For errors, they return the negative of the status code for easy `< 0` testing and extraction by the caller.
- Functions (usually low-level) that return pointers return `BAD_PTR` for hard alloc errors and
  `NULL_PTR` for normal and recoverable operations such as indicating end of iteration.


## Build Artifacts
In general, there are two bodies of code: an application or library product, and a tester executable.

Element            | Product         | Tester          |   
-----------        | -----------     | -----------     |
Code directory     | source_code     | test_code       | 
Output directory   | build_app       | build_test      |
Build script       | make_app.cmd    | make_test.cmd   |
Run script         | run_app.cmd     | run_test.cmd    |

