
@echo off
cls

rem Run leak test on the app.
rem This is a windows version using heob (https://sourceforge.net/projects/heob/).
rem For nx you would use valgrind.
rem There is still a tiny leak from the unit tester itself.

rem Setup path for qt and heob runtimes.
set PATH=%PATH%;C:\Qt\Tools\mingw530_32\bin;C:\Qt\5.11.2\mingw53_32\bin;C:\Qt\Tools\mingw530_32\bin;C:\Tools

rem Run the app.
heob64 -o1 \Dev\shadows\c-bag-of-tricks\build-test-Desktop_Qt_5_11_2_MinGW_32bit-Debug\debug\test

pause
