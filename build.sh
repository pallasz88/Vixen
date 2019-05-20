#!/bin/csh

mkdir build
cd build

cmake -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..

cmake --build . --target main -- -j %NUMBER_OF_PROCESSORS%
if  ( $? != 0) then
    echo **ERROR** main compilation failed
    exit /b %errorlevel% 
endif


cmake --build . --target unittest -- -j %NUMBER_OF_PROCESSORS%
if  ( $? != 0) then
    echo **ERROR** unittest compilation failed
    exit /b %errorlevel% 
endif


cmake --build . --target perfttest -- -j %NUMBER_OF_PROCESSORS%
if  ( $? != 0) then
    echo **ERROR** perfttest compilation failed
    exit /b %errorlevel% 
endif