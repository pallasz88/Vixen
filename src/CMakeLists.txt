cmake_minimum_required(VERSION 3.13)
set(SOURCE_FILES main.cpp)

add_subdirectory(vixen)
add_subdirectory(container)

add_executable(Vixen_0_0_5 ${SOURCE_FILES})
target_link_libraries(Vixen_0_0_5 PRIVATE Engine)
target_compile_features(Vixen_0_0_5 PRIVATE cxx_std_20)
target_include_directories(Vixen_0_0_5 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vixen
    ${CMAKE_CURRENT_SOURCE_DIR}/container/inl)

if (ENABLE_COVERAGE)
    message(STATUS "Generate GCOV input")
    target_compile_options(Vixen_0_0_5 PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
    target_link_libraries(Vixen_0_0_5 PRIVATE --coverage)
endif()
target_include_directories(Vixen_0_0_5 PRIVATE ${CMAKE_SOURCE_DIR}/src/container/inl)

install(TARGETS Vixen_0_0_5 DESTINATION bin)
