cmake_minimum_required(VERSION 3.5)
project(cbot_test)

# set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_STANDARD 11)

# needs -g to debug.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror")

include_directories("source")

# add_compile_definitions(HAVE_LIST=1)

# Source files.
add_executable(cbot_test
    source/pnut.cpp
    source/list.c
    source/dict.c
    source/state_machine.c
    source/stringx.c
    test/lock.c
    test/main.cpp
    test/test_list.cpp
    test/test_dict.cpp
    test/test_pnut.cpp
    test/test_sm.cpp
    test/test_stringx.cpp
    )
