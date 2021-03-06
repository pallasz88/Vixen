#!/bin/csh

mkdir build
cd build

set NUMBER_OF_PROCESSORS=`nproc`

cmake -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
cmake --build . --target install -- -j $NUMBER_OF_PROCESSORS
