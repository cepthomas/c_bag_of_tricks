:: Build and run the app with memory probe.

rem @echo off
cls

:: Setup dirs and files.
mkdir temp
del /F /Q temp\*.*
mkdir build

:: Build the app.
cd build
cmake -DUSE_PROBE="1"  -G "MinGW Makefiles"  ..
make

:: Run mem check. 
cbot_test.exe > ..\temp\probe.txt
cd ..
python proc_mem.py temp\probe.txt > temp\mem_out.txt
:: Go have a look.

rem pause
