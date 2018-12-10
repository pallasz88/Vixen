#define BOOST_TEST_MODULE boost_test_sequence

#include <boost/test/included/unit_test.hpp>
#include "board.h"
#include "move_generator.h"
#include "hash.h"

using namespace Vixen;

BOOST_AUTO_TEST_CASE(Test_bit)
{
    BOOST_TEST(1U == BIT(H1));
    BOOST_TEST(128U == BIT(A1));
    BOOST_TEST(1099511627776U == BIT(H6));
}

BOOST_AUTO_TEST_CASE(Test_Startposition)
{
    Vixen::Board board;
    const auto &bitBoards = board.GetBitBoards();
    BOOST_CHECK_EQUAL(bitBoards.at('K'), 8U);
    BOOST_CHECK_EQUAL(bitBoards.at('Q'), 16U);
    BOOST_CHECK_EQUAL(bitBoards.at('R'), 129U);
    BOOST_CHECK_EQUAL(bitBoards.at('B'), 36U);
    BOOST_CHECK_EQUAL(bitBoards.at('N'), 66U);
    BOOST_CHECK_EQUAL(bitBoards.at('P'), (uint64_t) RANK2);

    BOOST_CHECK_EQUAL(bitBoards.at('p'), (uint64_t) RANK7);
    BOOST_CHECK_EQUAL(bitBoards.at('n'), 4755801206503243776U);
    BOOST_CHECK_EQUAL(bitBoards.at('b'), 2594073385365405696U);
    BOOST_CHECK_EQUAL(bitBoards.at('r'), 9295429630892703744U);
    BOOST_CHECK_EQUAL(bitBoards.at('q'), 1152921504606846976U);
    BOOST_CHECK_EQUAL(bitBoards.at('k'), 576460752303423488U);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
}

BOOST_AUTO_TEST_CASE(Test_SetPosition)
{
    Vixen::Board board;
    board.SetBoard(TESTPOS1);
    board.PrintBoard();

    auto boards = board.GetBitBoards();
    BOOST_CHECK_EQUAL(boards.at('K'), 2U);
    BOOST_CHECK_EQUAL(boards.at('Q'), 524288U);
    BOOST_CHECK_EQUAL(boards.at('R'), 132U);
    BOOST_CHECK_EQUAL(boards.at('B'), 67109376U);
    BOOST_CHECK_EQUAL(boards.at('N'), 4096U);
    BOOST_CHECK_EQUAL(boards.at('P'), 103081542656U);

    BOOST_CHECK_EQUAL(boards.at('p'), 3802523525709824U);
    BOOST_CHECK_EQUAL(boards.at('n'), 536870912U);
    BOOST_CHECK_EQUAL(boards.at('b'), 5066549580791808U);
    BOOST_CHECK_EQUAL(boards.at('r'), 2594073385365405696U);
    BOOST_CHECK_EQUAL(boards.at('q'), 1152921504606846976U);
    BOOST_CHECK_EQUAL(boards.at('k'), 144115188075855872U);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);

    board.SetBoard(TESTPOS2);
    board.PrintBoard();

    boards = board.GetBitBoards();
    BOOST_CHECK_EQUAL(boards.at('K'), 8U);
    BOOST_CHECK_EQUAL(boards.at('Q'), 16U);
    BOOST_CHECK_EQUAL(boards.at('R'), 129U);
    BOOST_CHECK_EQUAL(boards.at('B'), 67108868U);
    BOOST_CHECK_EQUAL(boards.at('N'), 66U);
    BOOST_CHECK_EQUAL(boards.at('P'), 583424U);

    BOOST_CHECK_EQUAL(boards.at('p'), 62768920074911744U);
    BOOST_CHECK_EQUAL(boards.at('n'), 4611690416473899008U);
    BOOST_CHECK_EQUAL(boards.at('b'), 2594073385365405696U);
    BOOST_CHECK_EQUAL(boards.at('r'), 9295429630892703744U);
    BOOST_CHECK_EQUAL(boards.at('q'), 1152921504606846976U);
    BOOST_CHECK_EQUAL(boards.at('k'), 576460752303423488U);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
}

/*BOOST_AUTO_TEST_CASE(Test_pawnmovement)
{
    Vixen::Board board;
    board.SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList{"d4d5"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("8/4p3/2npP3/3P4/8/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList2{"d5c6"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList2, boost::test_tools::per_element());

    board.SetBoard("8/8/8/8/2pP4/8/8/8 b - d3 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList3{"c4c3", "c4d3"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList3, boost::test_tools::per_element());

    board.SetBoard("8/8/8/7p/p6P/P6p/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList4;
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList4, boost::test_tools::per_element());

    board.SetBoard("8/8/8/7p/p6P/P6p/7P/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList5;
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList5, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(Test_knightmovement)
{
    Vixen::Board board;
    board.SetBoard("8/8/8/8/3N4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList{"d4e6", "d4c6", "d4c2", "d4e2", "d4f5", "d4b5", "d4b3", "d4f3"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("N6N/8/8/1N4N1/8/8/8/N6N w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList2{"h1g3", "h1f2",
                                               "a1b3", "a1c2",
                                               "g5h7", "g5f7", "g5f3", "g5h3", "g5e6", "g5e4",
                                               "b5c7", "b5a7", "b5a3", "b5c3", "b5d6", "b5d4",
                                               "h8g6", "h8f7",
                                               "a8b6", "a8c7"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList2, boost::test_tools::per_element());

    board.SetBoard("8/8/8/8/3n4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList3{"d4e6", "d4c6", "d4c2", "d4e2", "d4f5", "d4b5", "d4b3", "d4f3"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList3, boost::test_tools::per_element());

    board.SetBoard("n6n/8/8/1n4n1/8/6B1/5QK1/n6n b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList4{"h1g3", "h1f2",
                                               "a1b3", "a1c2",
                                               "g5h7", "g5f7", "g5f3", "g5h3", "g5e6", "g5e4",
                                               "b5c7", "b5a7", "b5a3", "b5c3", "b5d6", "b5d4",
                                               "h8g6", "h8f7",
                                               "a8b6", "a8c7"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList4, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(Test_bishopmovement)
{
    Vixen::Board board;
    board.SetBoard("8/8/8/8/3B4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList{"d4c5", "d4b6", "d4a7", "d4e5", "d4f6", "d4g7", "d4h8", "d4c3", "d4b2",
                                              "d4a1", "d4e3", "d4f2", "d4g1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("8/Bp4pB/1P4P1/8/8/1p4p1/BP4PB/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList2{"h2g3", "h2g1", "a2b3", "a2b1", "h7g8", "a7b8"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList2, boost::test_tools::per_element());

    board.SetBoard("8/8/8/8/3b4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList3{"d4c5", "d4b6", "d4a7", "d4e5", "d4f6", "d4g7", "d4h8", "d4c3", "d4b2",
                                               "d4a1", "d4e3", "d4f2", "d4g1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList3, boost::test_tools::per_element());

    board.SetBoard("8/bp4pb/1P4P1/8/8/1p4p1/bP4Pb/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList4{"h2g1", "a2b1", "h7g8", "h7g6", "a7b8", "a7b6"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList4, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(Test_rookmovement)
{
    Vixen::Board board;
    board.SetBoard("8/8/8/8/3R4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList{"d4d5", "d4d6", "d4d7", "d4d8", "d4c4", "d4b4", "d4a4", "d4e4", "d4f4",
                                              "d4g4", "d4h4", "d4d3", "d4d2", "d4d1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList2{"h1g1", "h1f1", "h1e1", "h1d1", "h1c1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList2, boost::test_tools::per_element());


    board.SetBoard("8/8/8/8/3r4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList3{"d4d5", "d4d6", "d4d7", "d4d8", "d4c4", "d4b4", "d4a4", "d4e4", "d4f4",
                                               "d4g4", "d4h4", "d4d3", "d4d2", "d4d1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList3, boost::test_tools::per_element());

    board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList4{"a8b8", "a8a7", "a8a6", "a8a5",};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList4, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(Test_queenmovement)
{
    Vixen::Board board;
    board.SetBoard("8/8/8/8/3Q4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList{"d4d5", "d4d6", "d4d7", "d4d8", "d4c4", "d4b4", "d4a4", "d4e4", "d4f4",
                                              "d4g4", "d4h4", "d4d3", "d4d2", "d4d1", "d4c5", "d4b6", "d4a7", "d4e5",
                                              "d4f6", "d4g7", "d4h8", "d4c3", "d4b2", "d4a1", "d4e3", "d4f2", "d4g1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("8/8/8/8/3q4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList3{"d4d5", "d4d6", "d4d7", "d4d8", "d4c4", "d4b4", "d4a4", "d4e4", "d4f4",
                                               "d4g4", "d4h4", "d4d3", "d4d2", "d4d1", "d4c5", "d4b6", "d4a7", "d4e5",
                                               "d4f6", "d4g7", "d4h8", "d4c3", "d4b2", "d4a1", "d4e3", "d4f2", "d4g1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList3, boost::test_tools::per_element());

}

BOOST_AUTO_TEST_CASE(Test_kingmovement)
{
    Vixen::Board board;
    board.SetBoard("8/8/8/8/3K4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList{"d4d5", "d4c4", "d4e4", "d4d3", "d4c5", "d4e5", "d4c3", "d4e3"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("8/8/8/8/3k4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList3{"d4d5", "d4c4", "d4e4", "d4d3", "d4c5", "d4e5", "d4c3", "d4e3"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList3, boost::test_tools::per_element());

}
*/