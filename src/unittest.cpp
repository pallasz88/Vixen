#define BOOST_TEST_MODULE boost_test_sequence
#include <boost/test/included/unit_test.hpp>
#include "Board.h"
#include "MoveGenerator.h"
#include "defs.h"

BOOST_AUTO_TEST_CASE(Test_Startposition)
{
    Vixen::Board board;
    BOOST_CHECK_EQUAL(board.GetBitBoards().K, 8U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().Q, 16U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().R, 129U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().B, 36U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().N, 66U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().P, (uint64_t)RANK2);

    BOOST_CHECK_EQUAL(board.GetBitBoards().p, (uint64_t)RANK7);
    BOOST_CHECK_EQUAL(board.GetBitBoards().n, 4755801206503243776U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().b, 2594073385365405696U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().r, 9295429630892703744U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().q, 1152921504606846976U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().k, 576460752303423488U);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
}

BOOST_AUTO_TEST_CASE(Test_SetPosition)
{
    Vixen::Board board;
    board.SetBoard(TESTPOS1);
    board.PrintBoard();

    BOOST_CHECK_EQUAL(board.GetBitBoards().K, 2U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().Q, 524288U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().R, 132U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().B, 67109376U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().N, 4096U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().P, 103081542656U);

    BOOST_CHECK_EQUAL(board.GetBitBoards().p, 3802523525709824U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().n, 536870912U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().b, 5066549580791808U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().r, 2594073385365405696U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().q, 1152921504606846976U);
    BOOST_CHECK_EQUAL(board.GetBitBoards().k, 144115188075855872U);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
}

BOOST_AUTO_TEST_CASE(Test_pawnmovement)
{
    Vixen::Board board;
    board.SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList{"d4d5"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("8/8/2npQ3/3P4/8/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList2{"d5c6"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList2, boost::test_tools::per_element());

    board.SetBoard("8/8/5k2/8/2pP4/8/8/4K3 b - d3 0 1");
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
    std::vector<std::string> expectedMoveList2{ "h1g3", "h1f2",
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
    std::vector<std::string> expectedMoveList4{ "h1g3", "h1f2",
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
    std::vector<std::string> expectedMoveList{"d4c5", "d4b6", "d4a7", "d4e5", "d4f6", "d4g7", "d4h8", "d4c3", "d4b2", "d4a1", "d4e3", "d4f2", "d4g1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    board.SetBoard("8/Bp4pB/1P4P1/8/8/1p4p1/BP4PB/8 w - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList2{"h2g3", "h2g1", "a2b3", "a2b1", "h7g8", "a7b8"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList2, boost::test_tools::per_element());

    board.SetBoard("8/8/8/8/3b4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList3{"d4c5", "d4b6", "d4a7", "d4e5", "d4f6", "d4g7", "d4h8", "d4c3", "d4b2", "d4a1", "d4e3", "d4f2", "d4g1"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList3, boost::test_tools::per_element());

    board.SetBoard("8/bp4pb/1P4P1/8/8/1p4p1/bP4Pb/8 b - - 0 1");
    board.PrintBoard();
    std::vector<std::string> expectedMoveList4{"h2g1", "a2b1", "h7g8", "h7g6", "a7b8", "a7b6"};
    BOOST_TEST(board.GetMoveGenerator().GetMoveList() == expectedMoveList4, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(Test_bit)
{
    BOOST_TEST(256U == BIT(8));
    BOOST_TEST(1099511627776U == BIT(40));
}
