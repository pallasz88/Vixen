cmake_minimum_required(VERSION 3.13)

project(Vixen)
set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "x64")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(Vixen_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/vixen)
set(CMAKE_VERBOSE_MAKEFILE FALSE)
option(OPTIMIZE_FOR_NATIVE "Build with -march=native" OFF)
enable_testing()

# Set RPATH
if (CMAKE_HOST_UNIX)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/Extensions/lib:${CMAKE_INSTALL_PREFIX}/bin")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    MESSAGE(STATUS "CMAKE_INSTALL_RPATH: ${CMAKE_INSTALL_RPATH}")
endif ()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE AND OPTIMIZE_FOR_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    MESSAGE(STATUS "We are optimizing for this machine.")
endif ()

add_subdirectory(src)
add_subdirectory(unittest)
add_subdirectory(benchmark)
