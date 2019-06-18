
### This is the build using g++ for unit tests.

TEMPLATE = app
CONFIG += console warn_off
CONFIG -= app_bundle qt

SRC_PATH = "../source"

DEFINES += HAVE_LIST

SOURCES += \
    main.cpp \
    $$SRC_PATH/pnut.cpp \
    $$SRC_PATH/stringx.c \
    $$SRC_PATH/list.c \
    $$SRC_PATH/state_machine.c \
    test_pnut.cpp \
    test_list.cpp \
    test_stringx.cpp \
    test_sm.cpp \
    lock.c

HEADERS += \
    $$SRC_PATH/pnut.h \
    $$SRC_PATH/stringx.h \
    $$SRC_PATH/list.h \
    $$SRC_PATH/state_machine.h \
    lock.h
    
INCLUDEPATH += \
    $$SRC_PATH \
    .
