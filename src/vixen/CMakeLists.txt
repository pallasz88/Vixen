cmake_minimum_required(VERSION 3.13)
project(Vixen C CXX)

MESSAGE("STATUS: Building Vixen")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
        anti_slider.h
        anti_slider.cpp
        board.h
        board.cpp
        defs.h
        hash.h
        hash.cpp
        move_generator.h
        move_generator.cpp
        slider.cpp
        slider.h
        timer.h
        userinterface.h
        userinterface.cpp)

add_library(Vixen SHARED ${SOURCE_FILES})
target_link_libraries(Vixen)

install(TARGETS Vixen DESTINATION bin)
