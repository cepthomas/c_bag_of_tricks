
### This is the build using g++ for unit tests.

TEMPLATE = app
CONFIG += console warn_off
CONFIG -= app_bundle qt
CONFIG += warn_off

SOURCES += \
    main.cpp \
    source/pnut.cpp \
    source/list.c \
    source/state-machine.c \
    test/test-pnut.cpp \
    test/test-list.cpp \
    test/test-sm.cpp \
    test/lock.c

HEADERS += \
    source/pnut.h \
    source/list.h \
    source/state-machine.h \
    test/lock.h
    
INCLUDEPATH += \
    source \
    test
