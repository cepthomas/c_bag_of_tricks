
echo off
cls

mkdir build
cd build
rem del /F /Q *.*

cmake -G "MinGW Makefiles" ..

make

rem cbot_test.exe

cd ..

rem pause
