# Build the module with its unit tests.

cmake_minimum_required(VERSION 3.5)

project(cbot_test)

# This is a Windows 64 bit build. PATH must include \your\path\mingw64\bin.
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Project must be built for 64 bit")
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# add_compile_definitions(USE_PROBE=1)

# Header files.
include_directories(
    source
    pnut
    test
    )

# Source files.
add_executable(cbot_test
    source/private/logger.c
    source/private/list.c
    source/private/dict.c
    source/private/state_machine.c
    source/private/stringx.c
    source/private/mathutils.c
    source/private/cli.c
    source/private/ftimer.c
    source/private/stopwatch.c
//    source/private/timeanalyzer.c
    pnut/pnut.cpp
    test/main.cpp
    test/test_diagnostics.cpp
    test/test_logger.cpp
    test/test_list.cpp
    test/test_dict.cpp
    test/test_pnut.cpp
    test/test_stringx.cpp
    test/test_mathutils.cpp
    test/test_sm.cpp
    test/lock.c
    test/test_ftimer.cpp
    test/test_stopwatch.cpp
//    test/test_timeanalyzer.cpp
    test/test_cli.cpp
    )

target_link_libraries(cbot_test
    libwinmm.a
    )
