
# c_bag_of_tricks
- An ever-expanding collection of the C things I use repeatedly. The primary focus is on
  utilities for embedded systems. There are lots of other ways to do this but I find most to be over-complicated.
- There is some dynamic allocation, maybe I can make it all static eventually. No assert() are used.
- No dependencies on third party components.
- They all (except pnut) use the opaque pointer (pimpl) idiom.
- Runtime components are plain C99 so should build and run on any win or nx platform using any compiler.
- A VS Code workspace using mingw and CMake is supplied. Your PATH needs to include mingw.

![logo](felix.jpg)

# Conventions
- All C projects use this [naming convention](CONVENTIONS.md).
- The components in this collection generally follow the model described in [c_modular](https://github.com/cepthomas/c_modular).

## Tools
- Debugging memory management in composites like dict is difficult. Tools like heob and valgrind exist but I cobbled together
  something compatible with the CREATE/FREE macros. With this feature turned on (see run-probe.cmd), the app spews out all alloc() and
  free() calls, which is then fed through proc_mem.py to detect leaks and danglers. Crude but works ok.

# Components

## state_machine
- Semi-hierarchical state machine for C.
- Generates diagrams via [dot](https://www.graphviz.org/).
- See test_sm.cpp/lock.c for example of usage.

## list
- A simple doubly-linked list so we can have some rudimentary collections in C.
- See test_list.cpp for example of usage.

## dictionary
- A simple dictionary that supports either string or int keys and arbitrary value types.
- You can change the default bin size at compile time to match your data quantity and/or available space.
- See test_dict.cpp for example of usage.

## stringx
- Higher level string manipulation.
- See test_stringx.cpp for example of usage.

## cli
- Container for args with slicing and dicing into convenient parts.
- See test_cli.cpp for example of usage.

## pnut
Practically Nonexistent Unit Tester: A super lightweight unit test framework for C (or C++).
It has gone through many useful and successful iterations and may as well bring you joy also.
Inspired by [Quicktest](http://quicktest.sourceforge.net/) from long ago.

Output is to stdout. There are two formats currently supported:
- Readable text for humans.
- JUnit subset intended for consumption by your CI server. Not tested yet but looks nice.

It is plain C++ with a little bit of stl so will build and run on any win or nx platform using any compiler.

See test-pnut.cpp for an example of how to write unit tests and main.cpp of how to run them.
