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
        principal_variation.h
        principal_variation.cpp
        random.h
        slider.cpp
        slider.h
        small_map.h
        timer.h
        uci.h
        uci.cpp
        userinterface.h
        userinterface.cpp)

find_package(Threads)
add_library(Engine SHARED ${SOURCE_FILES})
target_link_libraries(Engine ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
    target_compile_options(Engine PRIVATE /W4)
else ()
    set(CLANG_WARNINGS
            -Wall
            -Wextra # reasonable and standard
            -Wshadow # warn the user if a variable declaration shadows one from a
#parent context
            -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
#non - virtual destructor.This helps catch hard to
#track down memory errors
            -Wold-style-cast # warn for c-style casts
            -Wcast-align # warn for potential performance problem casts
            -Wunused # warn on anything being unused
            -Woverloaded-virtual # warn if you overload (not override) a virtual
#function
            -Wpedantic # warn if non-standard C++ is used
            -Wconversion # warn on type conversions that may lose data
            -Wsign-conversion # warn on sign conversions
            -Wnull-dereference # warn if a null dereference is detected
            -Wdouble-promotion # warn if float is implicit promoted to double
            -Wformat=2 # warn on security issues around functions that format output
#(ie printf)
            )
    target_compile_options(Engine PRIVATE ${CLANG_WARNINGS})
endif ()

install(TARGETS Engine DESTINATION bin)
