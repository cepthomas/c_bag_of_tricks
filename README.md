# c-bag-of-tricks
An ever-expanding collection of the C things I seem to use repeatedly. Of course there are
lots of other ways to do this but I find most to be over-complicated.

No dependencies on third party components.

It is plain C99 so should build and run on any win or nx platform using any compiler.
A Qt Creator/QMake project using mingw is supplied (no Qt libs in output).

It makes use of my standby unit tester [pnut](https://github.com/cepthomas/pnut).

![logo](https://github.com/cepthomas/c-bag-of-tricks/blob/master/felix.jpg)

# state-machine
- Semi-hierarchical state machine for C.
- Generates diagrams via [dot](https://www.graphviz.org/).
- See test-sm.cpp/lock.c for an example of usage.

# list
- A simple doubly-linked list so we can have some rudimentary collections in C.
- See test-list.cpp for examples.

# Coming sooner or later
- String handling functions.
- Instrumented memory functions.
- Generic logging.
- Serialization - json and/or binary.

# License
https://github.com/cepthomas/c-bag-of-tricks/blob/master/LICENSE
