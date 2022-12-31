cmake_minimum_required(VERSION 3.13)

set(SOURCE_FILES unittest.cpp fixture.h)

include_directories(${PROJECT_SOURCE_DIR}/src/vixen)

set(Boost_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Extensions)
set(BOOST_LIBRARYDIR ${PROJECT_SOURCE_DIR}/Extensions/lib)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost)

if (Boost_FOUND)
    message(STATUS "Boost Include: ${Boost_INCLUDE_DIR}")
    message(STATUS "Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    
    add_executable(unittest ${SOURCE_FILES})
    add_test(NAME UnitTest COMMAND unittest WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(unittest PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(unittest PRIVATE cxx_std_20)

    add_executable(principal vixen_principal_variation_test.cpp)
    add_test(NAME PrincipalTest COMMAND principal WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(principal PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(principal PRIVATE cxx_std_20)
    
    add_executable(perft_start_position perft_start_position.cpp)
    add_test(NAME PerftTestStart COMMAND perft_start_position WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(perft_start_position PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(perft_start_position PRIVATE cxx_std_20)

    add_executable(perft_kiwipete_position perft_kiwipete_position.cpp)
    add_test(NAME PerftTestKiwi COMMAND perft_kiwipete_position WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(perft_kiwipete_position PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(perft_kiwipete_position PRIVATE cxx_std_20)

    add_executable(perft_rook_pawn_position perft_rook_pawn_position.cpp)
    add_test(NAME PerftTestRookPawn COMMAND perft_rook_pawn_position WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(perft_rook_pawn_position PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(perft_rook_pawn_position PRIVATE cxx_std_20)

    add_executable(perft_talk_chess_position perft_talk_chess_position.cpp)
    add_test(NAME PerftTestTalkChess COMMAND perft_talk_chess_position WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(perft_talk_chess_position PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(perft_talk_chess_position PRIVATE cxx_std_20)

    add_executable(perft_mirror_position perft_mirror_position.cpp)
    add_test(NAME PerftTestMirror COMMAND perft_mirror_position WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(perft_mirror_position PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(perft_mirror_position PRIVATE cxx_std_20)

    add_executable(perft_alternative_position perft_alternative_position.cpp)
    add_test(NAME PerftTestAlternative COMMAND perft_alternative_position WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    target_link_libraries(perft_alternative_position PRIVATE Engine ${Boost_LIBRARIES})
    target_compile_features(perft_alternative_position PRIVATE cxx_std_20)

    if (ENABLE_COVERAGE)
        message(STATUS "Generate GCOV input")
        target_compile_options(unittest PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
        target_link_libraries(unittest PRIVATE --coverage)

        target_compile_options(principal PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
        target_link_libraries(principal PRIVATE --coverage)
        
        target_compile_options(perft_start_position PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
        target_link_libraries(perft_start_position PRIVATE --coverage)

        target_compile_options(perft_kiwipete_position PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
        target_link_libraries(perft_kiwipete_position PRIVATE --coverage)

        target_compile_options(perft_rook_pawn_position PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
        target_link_libraries(perft_rook_pawn_position PRIVATE --coverage)

        target_compile_options(perft_mirror_position PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
        target_link_libraries(perft_mirror_position PRIVATE --coverage)

        target_compile_options(perft_alternative_position PRIVATE -O0 --coverage -fprofile-arcs -ftest-coverage -g)
        target_link_libraries(perft_alternative_position PRIVATE --coverage)

    endif()

    install(
        TARGETS 
            unittest
            principal
            perft_start_position
            perft_kiwipete_position
            perft_rook_pawn_position
            perft_talk_chess_position
            perft_mirror_position
            perft_alternative_position
        DESTINATION 
            bin
        )
    
endif ()
