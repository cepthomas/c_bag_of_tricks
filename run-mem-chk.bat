
@echo off
cls

rem Run leak test on the app.
rem This is a windows version using heob (https://sourceforge.net/projects/heob/).
rem For nx you would use valgrind.
rem There is still a tiny leak from the unit tester itself.

rem Setup path.
set PATH=%PATH%;C:\Tools

rem Run the app.
heob64 -o1 ..\build-cbot-test-Desktop_Qt_5_11_2_MinGW_32bit-Debug\debug\cbot-test

rem Usage: heob64 [OPTION]... APP [APP-OPTION]...
rem     -oX    heob output (0 = none, 1 = stdout, 2 = stderr, ... = file) [1]
rem     -PX    show process ID and wait [0]
rem     -cX    create new console [0]
rem     -pX    page protection (0 = off, 1 = after, 2 = before) [1]
rem     -fX    freed memory protection [0]
rem     -dX    monitor dlls [3]
rem     -hX    handle exceptions [1]
rem     -RX    raise breakpoint exception on allocation # [0]
rem     -rX    raise breakpoint exception on error [0]
rem     -FX    show full path [0]
rem     -lX    show leak details [1]
rem     -zX    minimum leak size [0]
rem     -kX    control leak recording [0]
rem     -LX    show leak contents [0]
rem     -H     show full help

rem pause
