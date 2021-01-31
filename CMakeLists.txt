cmake_minimum_required(VERSION 3.5)
project(cbot_test)

set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_STANDARD 11)

# needs -g to debug. TODO clean up flags when done. -Wno-unused-variable
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wno-unused-variable")

include_directories("cbot_module" "test")

# add_compile_definitions(HAVE_LIST=1)

# Source files.
add_executable(cbot_test
    cbot_module/private/list.c
    cbot_module/private/dict.c
    cbot_module/private/state_machine.c
    cbot_module/private/stringx.c
    cbot_module/private/pnut.cpp
    test/main.cpp
    test/test_list.cpp
    test/test_dict.cpp
    test/test_pnut.cpp
    test/test_sm.cpp
    test/test_stringx.cpp
    test/lock.c
    )


# Copy test files.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/hemingway_short.txt  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
