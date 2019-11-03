#!/bin/bash

rm -fr build
mkdir build
cp CMakeLists.txt build/CMakeLists.txt
cp -r source_code build/.
cp -r image_data build/.
cd build
cmake .
make
