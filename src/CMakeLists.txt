cmake_minimum_required(VERSION 3.13)
project(main)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)

set(DRMINGW_LIBS
        dbghelp.dll
        exchndl.dll
        mgwhelp.dll
        symsrv.dll
        symsrv.yes)

add_subdirectory(vixen)
include_directories(vixen ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw)

add_executable(main ${SOURCE_FILES})
target_link_libraries(main Vixen ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw/lib/exchndl.dll)

install(TARGETS main DESTINATION bin)
if (CMAKE_HOST_WIN32)
foreach (files ${DRMINGW_LIBS})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/drmingw/lib/${files} DESTINATION bin)
endforeach ()
endif()
