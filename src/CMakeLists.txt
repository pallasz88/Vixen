cmake_minimum_required(VERSION 3.13)
project(main)

MESSAGE("Status: Building main")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

include_directories(vixen)

add_subdirectory(vixen)

add_executable(Vixen_0_0_5 ${SOURCE_FILES})
target_link_libraries(Vixen_0_0_5 Engine pthread)

install(TARGETS Vixen_0_0_5 DESTINATION bin)
