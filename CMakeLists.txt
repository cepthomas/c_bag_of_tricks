# Build the module with its unit tests.

cmake_minimum_required(VERSION 3.5)

project(cbot_test)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# add_compile_definitions(USE_PROBE=1)

# Header files.
include_directories(
    source_code
    test_code
    pnut
    )

# Source files.
add_executable(cbot_test
    source_code/private/logger.c
    source_code/private/list.c
    source_code/private/dict.c
    source_code/private/state_machine.c
    source_code/private/stringx.c
    source_code/private/mathutils.c
    source_code/private/ftimer.c
    source_code/private/stopwatch.c
    test_code/main.cpp
    test_code/test_diagnostics.cpp
    test_code/test_logger.cpp
    test_code/test_list.cpp
    test_code/test_dict.cpp
    test_code/test_pnut.cpp
    test_code/test_stringx.cpp
    test_code/test_mathutils.cpp
    test_code/test_sm.cpp
    test_code/lock.c
    test_code/test_ftimer.cpp
    test_code/test_stopwatch.cpp
    pnut/pnut.cpp
    )

target_link_libraries(cbot_test
    libwinmm.a
    )
