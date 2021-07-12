:: Run the app with memory probe.

rem @echo off
cls

:: Setup dirs and files.
mkdir temp
del /F /Q temp\*.*
rem cd build

:: Run mem check. 
..\build\cbot_test.exe > temp\probe.txt
rem cd ..
python proc_mem.py temp\probe.txt > temp\mem_out.txt

:: Have a look.
type temp\mem_out.txt

pause
