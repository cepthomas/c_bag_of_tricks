
### This is the build using g++ for unit tests.

TEMPLATE = app
CONFIG += console warn_off
CONFIG -= app_bundle qt


SOURCES += \
    main.cpp \
    source/pnut.cpp \
    source/list.c \
    source/state_machine.c \
    test/test_pnut.cpp \
    test/test_list.cpp \
    test/test_sm.cpp \
    test/lock.c

HEADERS += \
    source/pnut.h \
    source/list.h \
    source/state_machine.h \
    test/lock.h
    
INCLUDEPATH += \
    source \
    test
