TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle qt

UNIT_TEST = true

### Common stuff.
SOURCES += \
    source/list.c \
    source/state-machine.c \
    test/lock.c

HEADERS += \
    source/list.h \
    source/state-machine.h \
    test/lock.h

INCLUDEPATH += source

$$UNIT_TEST {
    #message(g++ for unit tests)

    SOURCES += \
        main.cpp \
        test/test-list.cpp \
        test/test-sm.cpp \
        test/test-pnut.cpp

    HEADERS += \
        source/pnut.h

    #TODO fix these:
    CONFIG += warn_off
    #QMAKE_CXXFLAGS += -Wold-style-cast -Wzero-as-null-pointer-constant
    #QMAKE_CXXFLAGS_WARN_ON -= -Wold-style-cast -Wzero-as-null-pointer-constant

} else {
    #message(gcc for C99 compliance)

    CONFIG += strict_c
    CONFIG -= c11
    SOURCES += \
        main.c \
}
