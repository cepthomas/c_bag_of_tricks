cmake_minimum_required(VERSION 3.5)

project(cbot_test)

set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_STANDARD 11)

# needs -g to debug.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")


include_directories(
    source
    pnut
    test
    )

# Source files.
add_executable(cbot_test
    source/private/common.c
    source/private/logger.c
    source/private/list.c
    source/private/dict.c
    source/private/state_machine.c
    source/private/stringx.c
    pnut/pnut.cpp
    test/main.cpp
    test/test_common.cpp
    test/test_logger.cpp
    test/test_list.cpp
    test/test_dict.cpp
    test/test_pnut.cpp
    test/test_stringx.cpp
    test/test_sm.cpp
    test/lock.c
    )
