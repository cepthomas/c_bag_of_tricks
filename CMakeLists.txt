cmake_minimum_required(VERSION 3.5)
project(cbot_test)

# set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_STANDARD 11)

# Source files.
include_directories("source")
add_compile_definitions(HAVE_LIST=1)

add_executable(cbot_test
        source/pnut.cpp
        source/list.c
        source/state_machine.c
        source/stringx.c
        test/lock.c
        test/main.cpp
        test/test_list.cpp
        test/test_pnut.cpp
        test/test_sm.cpp
        test/test_stringx.cpp
        )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror") # needs -g!
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
