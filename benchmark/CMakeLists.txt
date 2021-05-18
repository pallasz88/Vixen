cmake_minimum_required(VERSION 3.13)
project(benchmark)

MESSAGE("Status: Building benchmark")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES benchmark.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Extensions/benchmark build)

add_executable(vixen_benchmark ${SOURCE_FILES})
target_include_directories(vixen_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/vixen)
if (MSVC)
    set(BENCHMARK_LINK_LIBS Engine benchmark::benchmark)
else()
    set(BENCHMARK_LINK_LIBS Engine pthread benchmark::benchmark)
endif()

target_link_libraries(vixen_benchmark ${BENCHMARK_LINK_LIBS})

install(TARGETS vixen_benchmark DESTINATION bin)
