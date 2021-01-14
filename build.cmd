
echo off
cls

rem mkdir _build
rem cd _build
rem del /F /Q *.*
cmake -G "MinGW Makefiles" .

make

rem cbot_test.exe

rem cd ..

rem pause
