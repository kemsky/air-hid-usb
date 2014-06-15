@echo off
rem gcc -Wall -shared hid.cpp air.cpp -o hidapi.dll -lhid -lsetupapi -L. -lFlashRuntimeExtensions -lstdc++
mingw32-make clean && mingw32-make all