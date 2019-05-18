@echo off

mkdir build
cd build

cmake -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
cmake --build . --target main -- -j %NUMBER_OF_PROCESSORS%

if %errorlevel% neq 0 (
    echo **ERROR** main compilation failed
    exit /b %errorlevel% 
)

cmake --build . --target unittest -- -j %NUMBER_OF_PROCESSORS%

if %errorlevel% neq 0 (
    echo **ERROR** unittest compilation failed
    exit /b %errorlevel% 
)

cmake --build . --target perfttest -- -j %NUMBER_OF_PROCESSORS%

if %errorlevel% neq 0 (
    echo **ERROR** perfttest compilation failed
    exit /b %errorlevel% 
)