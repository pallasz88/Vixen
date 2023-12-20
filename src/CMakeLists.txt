cmake_minimum_required(VERSION 3.13)
project(main)

MESSAGE(STATUS "Building main")

set(SOURCE_FILES main.cpp)

include_directories(vixen)

add_subdirectory(vixen)
add_subdirectory(container)

add_executable(Vixen_0_0_5 ${SOURCE_FILES})
if(MSVC)
target_link_libraries(Vixen_0_0_5 PRIVATE Engine)
else()
target_link_libraries(Vixen_0_0_5 PRIVATE Engine pthread)
endif()
target_compile_features(Vixen_0_0_5 PRIVATE cxx_std_20)

if (ENABLE_COVERAGE)
    message(STATUS "Generate GCOV input")
    target_compile_options(Vixen_0_0_5 PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
    target_link_libraries(Vixen_0_0_5 PRIVATE --coverage)
endif()
target_include_directories(Vixen_0_0_5 PRIVATE ${CMAKE_SOURCE_DIR}/src/container/inl)

install(TARGETS Vixen_0_0_5 DESTINATION bin)
