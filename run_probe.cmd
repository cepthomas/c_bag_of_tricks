:: Run the app with memory probe.

rem @echo off
cls

:: Run mem check.
cd build
cbot_test.exe > probe.txt
python ..\proc_mem.py probe.txt > mem_out.txt

:: Have a look.
type mem_out.txt
cd ..

pause
