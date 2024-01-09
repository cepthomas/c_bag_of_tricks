echo off
rem cls

:: Setup dirs and files.
mkdir build
pushd build
rem del /F /Q *.*

:: Build the app.
cmake -G "MinGW Makefiles" ..
make

popd

:: Copy test files.
copy test\files\* build
