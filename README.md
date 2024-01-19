
# C Bag Of Tricks

- An ever-expanding collection of the C things I use repeatedly. The primary focus is on
  utilities for embedded systems. There are lots of other ways to do this but I find most to be over-complicated.
- There is some dynamic allocation, maybe I can make it all static eventually. No assert() are used.
- No dependencies on third party components.
- They all (except pnut) use the opaque pointer (pimpl) idiom.
- Runtime components are plain C99 so should build and run on any win or nx platform using any compiler.
- Test code is Windows 64 bit build using CMake. PATH must include ...\mingw64\bin.
- For conventions see [c_modular](https://github.com/cepthomas/c_modular/blob/master/README.md).

![logo](felix.jpg)

# Components

## status

Rather than add a whole new status and error handling system, this (ab)uses existing C patterns:
- Most functions return an int status code consisting of standard errno values, plus
  additional cbot specific values starting at 200.
- Some functions return numerical values like count/length/index which are always >= 0.
  For errors they return the negative of the offending errno for easy testing and extraction by the caller.
- Functions (usually low-level) that return pointers return BAD_PTR for hard alloc errors and
  NULL for normal operations such as indicating end of iteration.

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
- Container for args with slicing and dicing into convenient parts. Windows only.
- See test_cli.cpp for example of usage.

## ftimer
A fast (msec) timer based on Windows multimedia timer. The win multimedia timer is erratic with possible
errors of many msec.

This component attempts to reduce the error by running at one msec and managing the requested periods manually.
This is accomplished by using a stopwatch (modelled after the C# class) to actually measure the elapsed time
rather than trust the mm timer period. It seems to be an improvement with roughly +-1 msec precision on an
unloaded system. That should be good enough for midi.

See `test_ftimer.cpp` for usage.

## stopwatch

Stopwatch similar to the .NET component. [Beware of QueryPerformanceCounter()](https://www.virtualdub.org/blog2/entry_106.html)

## timeanalyzer

Takes repeated time samples and performs some basic statistics.


# Tools

## pnut
Practically Nonexistent Unit Tester: A super lightweight unit test framework for C (or C++).
It has gone through many useful and successful iterations and may as well bring you joy also.
Inspired by [Quicktest](http://quicktest.sourceforge.net/) from long ago.

Output is to stdout. There are two formats currently supported:
- Readable text for humans.
- JUnit subset intended for consumption by your CI server. Not tested yet but looks nice.

It is plain C++ with a little bit of stl so will build and run on any win or nx platform using any compiler.

See test-pnut.cpp for an example of how to write unit tests and main.cpp of how to run them.

## diagnostics
- Debugging memory management in composites like dict is difficult. Tools like heob and valgrind exist but I cobbled together
  something compatible with the CREATE/FREE macros. With this feature turned on (see run-probe.cmd), the app spews out all alloc() and
  free() calls, which is then fed through proc_mem.py to detect leaks and danglers. Crude but works ok.
