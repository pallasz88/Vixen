@echo off

cmake -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" .
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

mingw32-make install