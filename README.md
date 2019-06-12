# c-bag-of-tricks
An ever-expanding collection of the C things I seem to use repeatedly. Of course there are
lots of other ways to do this but I find most to be over-complicated.

No dependencies on third party components.

Runtime components are plain C99 so should build and run on any win or nx platform using any compiler.
A Qt Creator/QMake project using mingw is supplied (no Qt libs in output).

![logo](https://github.com/cepthomas/c-bag-of-tricks/blob/master/felix.jpg)

# state-machine
- Semi-hierarchical state machine for C.
- Generates diagrams via [dot](https://www.graphviz.org/).
- See test-sm.cpp/lock.c for an example of usage.

# list
- A simple doubly-linked list so we can have some rudimentary collections in C.
- See test-list.cpp for examples.

# pnut
Practically Nonexistent Unit Tester: A super lightweight unit test framework for C/C++. It has gone
through many useful and successful iterations and may as well bring you joy also.
Inspired by [Quicktest](http://quicktest.sourceforge.net/) from long ago.

Output is to stdout. There are two formats currently supported:
- Readable text for humans.
- JUnit subset intended for consumption by your CI server. Not tested yet but looks nice.

It is plain C++ with a little bit of stl so will build and run on any win or nx platform using any compiler. A Qt Creator/QMake
project using mingw is supplied (no Qt libs in output).

See test-pnut.cpp for an example of how to write unit tests and main.cpp of how to run them.

The original intent was to provide a way to unit test algorithmic parts of embedded code in an isolated
environment. To that end, the system must be designed so as to abstract the hardware components.
An example showing how to use this for embedded applications is [c-modular](https://github.com/cepthomas/c-modular).


# Coming sooner or later
- String handling functions.
- Generic logging.
- Serialization - json and/or binary.

# License
https://github.com/cepthomas/c-bag-of-tricks/blob/master/LICENSE
