
# c-bag-of-tricks
- An ever-expanding collection of the C things I seem to use repeatedly. The primary focus is on
  utilities for embedded systems. There are lots of other ways to do this but I find most to be over-complicated.
- There is some dynamic allocation, maybe I can make it all static eventually. No assert() are used.
- No dependencies on third party components.
- They all (except pnut) use the opaque pointer (pimpl) idiom.
- Runtime components are plain C99 so should build and run on any win or nx platform using any compiler.
- A VS Code workspace using mingw and CMake is supplied. Your PATH needs to include mingw.

![logo](felix.jpg)

# Components
- The components in this collection generally follow the model put forth in [c-modular](https://github.com/cepthomas/c-modular).
- If a function returns a pointer, the client now owns it and is responsible for destroying it. This doesn't apply to const pointers.

## state_machine
- Semi-hierarchical state machine for C.
- Generates diagrams via [dot](https://www.graphviz.org/).
- See test_sm.cpp/lock.c for example of usage.

## list
- A simple doubly-linked list so we can have some rudimentary collections in C.
- See test_list.cpp for example of usage.

## dictionary
- TODO dict doc

## stringx
- Higher level string manipulation.
- See test_stringx.cpp for example of usage.

# Error Handling
In an embedded system, most real errors are considered unrecoverable. Things like handling comm timeouts should be considered
normal behavior and handled accordingly. So cbot errors are very bad and usually result in hard crash/reset. This of course should never
happen because they have all been caught in unit and integration testing, right?

Rather than add a whole new error handling system, cbot uses existing C patterns:
- Functions that return pointers return NULL for errors.
- Functions that return things like counts (0 is valid) return -1 for errors.
- Functions that return status return -1 for errors, 0 for success, >0 for failure (logical not error).
- When errors occur, cbot sets errno accordingly.
- common.h defines some macros:
    - Return values for ints and ponters.
    - Macros for creating typed objects - CREATE_INST(), CREATE_STR().
    - Macros for validating pointers - VALIDATE_PTR1(), VALIDATE_PTR2(). Note that these use early returns to keep the if-nesting reasonable.
      Normally I hate early returns but in this case the pluses outweigh.

# pnut
Practically Nonexistent Unit Tester: A super lightweight unit test framework for C (or C++).
It has gone through many useful and successful iterations and may as well bring you joy also.
Inspired by [Quicktest](http://quicktest.sourceforge.net/) from long ago.

Output is to stdout. There are two formats currently supported:
- Readable text for humans.
- JUnit subset intended for consumption by your CI server. Not tested yet but looks nice.

It is plain C++ with a little bit of stl so will build and run on any win or nx platform using any compiler.

See test-pnut.cpp for an example of how to write unit tests and main.cpp of how to run them.

The original intent was to provide a way to unit test algorithmic parts of embedded code in an isolated
environment. To that end, the system must be designed so as to abstract the hardware components.
An example showing how to use this for embedded applications is [c-modular](https://github.com/cepthomas/c-modular).
