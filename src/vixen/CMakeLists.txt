cmake_minimum_required(VERSION 3.13)
project(Vixen C CXX)

MESSAGE("STATUS: Building Vixen")

set(SOURCE_FILES
        anti_slider.hpp
        anti_slider.inl
        anti_slider.cpp
        board.hpp
        board.cpp
        defs.hpp
        engine.hpp
        engine.cpp
        hash.hpp
        hash.cpp
        move_generator.hpp
        move_generator.cpp
        principal_variation.hpp
        principal_variation.cpp
        random.hpp
        slider.cpp
        slider.inl
        slider.hpp
        timer.hpp
        uci.hpp
        uci.cpp
        userinterface.hpp
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

target_compile_features(Engine PRIVATE cxx_std_20)

install(TARGETS Engine DESTINATION bin)
