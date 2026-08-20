@echo off

mkdir build
cd build

cmake -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
if errorlevel 1 exit /b %errorlevel%
cmake --build . --target install --parallel
if errorlevel 1 exit /b %errorlevel%