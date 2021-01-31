
rem @echo off
cls

rem Build and run the app w/probe.

mkdir build

cd build

cmake -DUSE_PROBE="1"  -G "MinGW Makefiles"  ..

make

cbot_test.exe > ..\temp\probe.txt
cd ..

python proc-mem.py temp\probe.txt > temp\mem_out.txt


rem heob version
rem heob32 -D -x cbot_test.exe > run_out.txt

rem pause