cmake_minimum_required(VERSION 3.12)
project(Vixen C CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        board.h
        board.cpp
        defs.h
        hash.h
        hash.cpp
        move_generator.h
        move_generator.cpp)

set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Extensions)
set(BOOST_LIBRARYDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Extensions/lib)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.66 COMPONENTS unit_test_framework REQUIRED)

if (Boost_FOUND)
    message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(status "** Boost Libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    add_library(Vixen SHARED ${SOURCE_FILES})
    target_link_libraries(Vixen ${Boost_LIBRARIES})
endif ()

install(TARGETS Vixen DESTINATION bin)
get_filename_component(UTF_BASE_NAME_DEBUG ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG} NAME_WE)
install(FILES ../../Extensions/lib/${UTF_BASE_NAME_DEBUG}.a
        DESTINATION bin
        CONFIGURATIONS Debug
        )