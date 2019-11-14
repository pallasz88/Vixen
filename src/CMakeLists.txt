cmake_minimum_required(VERSION 3.13)
project(main)

MESSAGE("Status: Building main")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

include_directories(vixen)

add_subdirectory(vixen)

add_executable(Vixen ${SOURCE_FILES})
target_link_libraries(Vixen Engine pthread)

install(TARGETS Vixen DESTINATION bin)
