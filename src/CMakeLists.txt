cmake_minimum_required(VERSION 3.13)
project(main)

MESSAGE("Status: Building main")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)

set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions)
set(BOOST_LIBRARYDIR ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/lib)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.69 COMPONENTS thread REQUIRED)

if (Boost_FOUND)
    message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(status "** Boost Libraries: ${Boost_LIBRARIES}")
    include_directories(vixen ${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif ()

if (CMAKE_HOST_WIN32)
    set(DRMINGW_LIBS
            dbghelp.dll
            exchndl.dll
            mgwhelp.dll
            symsrv.dll
            symsrv.yes)
endif ()

add_subdirectory(vixen)
if (CMAKE_HOST_WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw)
endif ()

add_executable(main ${SOURCE_FILES})
if (CMAKE_HOST_WIN32)
    target_link_libraries(main
            Vixen
            ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw/lib/exchndl.dll
            ${Boost_LIBRARIES})
else ()
    target_link_libraries(main Vixen ${Boost_LIBRARIES})
endif ()

install(TARGETS main DESTINATION bin)

if (CMAKE_HOST_WIN32)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/lib/libboost_thread-mgw72-mt-x64-1_69.dll DESTINATION bin)
else ()
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/lib/libboost_thread.so.1.69.0 DESTINATION bin)
endif ()
if (CMAKE_HOST_WIN32)
    foreach (files ${DRMINGW_LIBS})
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw/lib/${files} DESTINATION bin)
    endforeach ()
endif ()
