#!/bin/bash

rm -fr build
rm -fr ~/cmpt433/public/project
mkdir build
mkdir ~/cmpt433/public/project
cp CMakeLists.txt build/CMakeLists.txt
cp -r source_code build/.
cp -r image_data build/.
cp ../gaming-libraries/multicast.h build/source_code/.
cd build
cmake -D CMAKE_C_COMPILER=arm-linux-gnueabihf-g++ -D CMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ .
make
