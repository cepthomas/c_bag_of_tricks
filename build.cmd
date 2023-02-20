:: Standard build - used by VS Code task.

:: Setup dirs and files.
mkdir build
cd build
rem del /F /Q *.*

:: Build the app.
cmake -G "MinGW Makefiles" ..
make
cd ..

rem Copy test files.
copy test\files\* build

cd build
cbot_test
cd ..

rem pause
