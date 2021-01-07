@echo off

mkdir build
cd build

cmake -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON ..
cmake --build . --target install -- -j %NUMBER_OF_PROCESSORS%