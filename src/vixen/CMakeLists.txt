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
        engine.h
        engine.cpp
        hash.h
        hash.cpp
        move_generator.h
        move_generator.cpp
        random.h
        slider.cpp
        slider.h
        timer.h
        uci.h
        uci.cpp
        userinterface.h
        userinterface.cpp)

add_library(Vixen SHARED ${SOURCE_FILES})
target_link_libraries(Vixen)

if (MSVC)
    target_compile_options(Vixen PRIVATE /W4 /WX)
else ()
    target_compile_options(Vixen PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
            -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion
            -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference
            -Wuseless-cast -Wdouble-promotion -Wformat=2)
endif ()

install(TARGETS Vixen DESTINATION bin)
