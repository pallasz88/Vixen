cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "x64")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(Vixen_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/vixen)

add_subdirectory(src)
add_subdirectory(unittest)