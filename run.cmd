
@echo off
cls

rem Run leak test on the app.

rem Run the app.
cd build

rem heob version
rem heob32 -D -x cbot_test.exe > ..\run_out.txt

rem probe version
cbot_test.exe | python ..\proc-mem.py > ..\run_out.txt

cd ..

pause
