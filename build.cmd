echo off
cls

:: Setup dirs and files.
mkdir build
pushd build
rem del /F /Q *.*

:: Build the app.
cmake -G "MinGW Makefiles" ..
make

rem cbot_test

popd

:: Copy test files.
copy test\files\* build

rem pause
