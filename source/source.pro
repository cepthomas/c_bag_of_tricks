
### This is the build using gcc for C99 compliance check.

TEMPLATE = app
CONFIG += console strict_c
CONFIG -= app_bundle qt c11
# Keep it honest - treat warnings as errors.
QMAKE_CFLAGS += -Werror

DEFINES += HAVE_LIST

SOURCES += \
    main.c \
    list.c \
    stringx.c \
    state_machine.c

HEADERS += \
    list.h \
    stringx.h \
    state_machine.h
    
INCLUDEPATH += \
    .
