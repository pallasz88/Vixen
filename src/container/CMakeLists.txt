project(container)

enable_testing()
add_library(container SHARED
                src/sorted_map.hpp
                src/sorted_map.inl
                src/sorted_set.hpp
                src/sorted_set.inl)
set_target_properties(container PROPERTIES LINKER_LANGUAGE CXX)            

# include(GoogleTest)
# gtest_add_tests(TARGET
#                     container_test
#                 SOURCES
#                     test/test_main.cpp
#                     test/test_sorted_map.cpp
#                     test/test_sorted_set.cpp
#                 WORKING_DIRECTORY
#                     ${CMAKE_SOURCE_DIR}/bin
#                 )

add_executable(container_test
                test/test_main.cpp
                test/test_sorted_map.cpp
                test/test_sorted_set.cpp)

add_test(NAME ContainerTest COMMAND container_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

target_include_directories(container_test PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(container_test PRIVATE gtest gtest_main pthread)

target_compile_features(container PRIVATE cxx_std_20)
target_compile_features(container_test PRIVATE cxx_std_20)

install(TARGETS container DESTINATION bin)
install(TARGETS container_test DESTINATION bin)