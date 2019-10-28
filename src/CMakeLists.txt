cmake_minimum_required(VERSION 3.13)
project(main)

MESSAGE("Status: Building main")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

include_directories(vixen)

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
            ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw/lib/exchndl.dll)
else ()
    target_link_libraries(main Vixen pthread)
endif ()

install(TARGETS main DESTINATION bin)

if (CMAKE_HOST_WIN32)
    foreach (files ${DRMINGW_LIBS})
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw/lib/${files} DESTINATION bin)
    endforeach ()
endif ()
