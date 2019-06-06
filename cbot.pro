TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle qt

UNIT_TEST = true

### Common stuff.
SOURCES += \
    source/list.c \
    source/state-machine.c \

HEADERS += \
    source/list.h \
    source/state-machine.h

INCLUDEPATH += \
    source \
    test

$$UNIT_TEST {
    #message(g++ for unit tests)

    SOURCES += \
        main.cpp \
        test/test-list.cpp \
        test/test-sm.cpp \
        test/lock.c

    HEADERS += \
        test/pnut.h \
        test/lock.h

    CONFIG += warn_off
    #QMAKE_CXXFLAGS_WARN_ON -= -Wold-style-cast -Wzero-as-null-pointer-constant

} else {
    #message(gcc for C99 compliance)

    CONFIG += strict_c
    CONFIG -= c11
    QMAKE_CFLAGS += -Werror

    SOURCES += \
        main.c \
}
