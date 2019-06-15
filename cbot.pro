
### This is the build using gcc for C99 compliance check.

TEMPLATE = app
CONFIG += console strict_c
CONFIG -= app_bundle qt c11
# Keep it honest - treat warnings as errors.
QMAKE_CFLAGS += -Werror

SOURCES += \
    main.c \
    source/list.c \
    source/state_machine.c

HEADERS += \
    source/list.h \
    source/state_machine.h
    
INCLUDEPATH += \
    source
