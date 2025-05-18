@echo off
if not exist build mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=g++ -G "MinGW Makefiles" -S .. -B .
cmake --build . --target all -j 14