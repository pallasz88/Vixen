cmake_minimum_required(VERSION 3.13)
project(main)

MESSAGE(STATUS "Building main")

set(SOURCE_FILES main.cpp)
set(EXECUTABLE Vixen_0_0_5)

add_subdirectory(vixen)
add_subdirectory(container)

add_executable(${EXECUTABLE} ${SOURCE_FILES})
target_include_directories(${EXECUTABLE} PRIVATE vixen)

if(MSVC)
target_link_libraries(${EXECUTABLE} PRIVATE Engine)
else()
target_link_libraries(${EXECUTABLE} PRIVATE Engine pthread)
endif()
target_compile_features(${EXECUTABLE} PRIVATE cxx_std_20)

if (ENABLE_COVERAGE)
    message(STATUS "Generate GCOV input")
    target_compile_options(${EXECUTABLE} PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
    target_link_libraries(${EXECUTABLE} PRIVATE --coverage)
endif()

install(TARGETS ${EXECUTABLE} DESTINATION bin)
