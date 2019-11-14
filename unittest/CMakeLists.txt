cmake_minimum_required(VERSION 3.13)
project(unittest)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES unittest.cpp fixture.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/vixen)

set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions)
set(BOOST_LIBRARYDIR ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/lib)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.69 COMPONENTS unit_test_framework REQUIRED)

if (Boost_FOUND)
    message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(status "** Boost Libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(unittest ${SOURCE_FILES})
    add_executable(perfttest perfttest.cpp)
    target_link_libraries(unittest Engine ${Boost_LIBRARIES})
    target_link_libraries(perfttest Engine ${Boost_LIBRARIES})
endif ()

install(TARGETS perfttest DESTINATION bin)
install(TARGETS unittest perfttest DESTINATION bin)