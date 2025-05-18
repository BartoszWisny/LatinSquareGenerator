#!/bin/bash
pip install conan
conan profile detect --force
rm -rf ../build
mkdir ../build
conan install . --output-folder=../build --build=missing
cd ../build
cmake ../LinuxCompilation -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .