rem Standard build - called by VS Code.

echo off
cls

mkdir build
cd build
rem del /F /Q *.*

cmake -G "MinGW Makefiles" ..

make

cd ..

rem pause
