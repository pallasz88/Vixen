#define BOOST_TEST_MODULE boost_test_sequence
#include <boost/test/included/unit_test.hpp>
#include "Board.h"
#include "MoveGenerator.h"
#include "defs.h"

BOOST_AUTO_TEST_CASE(Test_Startposition)
{
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().K, 8U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().Q, 16U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().R, 129U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().B, 36U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().N, 66U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().P, (uint64_t)RANK2);

    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().p, (uint64_t)RANK7);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().n, 4755801206503243776U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().b, 2594073385365405696U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().r, 9295429630892703744U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().q, 1152921504606846976U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().k, 576460752303423488U);

    BOOST_CHECK_EQUAL(CURRENT_BOARD()->IsWhiteToMove(), true);
}

BOOST_AUTO_TEST_CASE(Test_SetPosition)
{
    CURRENT_BOARD()->SetBoard(TESTPOS1);
    CURRENT_BOARD()->PrintBoard();

    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().K, 2U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().Q, 524288U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().R, 132U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().B, 67109376U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().N, 4096U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().P, 103081542656U);

    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().p, 3802523525709824U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().n, 536870912U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().b, 5066549580791808U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().r, 2594073385365405696U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().q, 1152921504606846976U);
    BOOST_CHECK_EQUAL(CURRENT_BOARD()->GetBitBoards().k, 144115188075855872U);

    BOOST_CHECK_EQUAL(CURRENT_BOARD()->IsWhiteToMove(), true);
}

BOOST_AUTO_TEST_CASE(Test_pawnmovement)
{
    CURRENT_BOARD()->SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
    CURRENT_BOARD()->PrintBoard();
    std::vector<std::string> expectedMoveList{"d4d5"};
    BOOST_TEST(CURRENT_MOVES()->GetMoveList() == expectedMoveList, boost::test_tools::per_element());

    CURRENT_MOVES()->CleanUp();

    CURRENT_BOARD()->SetBoard("8/8/2npQ3/3P4/8/8/8/8 w - - 0 1");
    CURRENT_BOARD()->PrintBoard();
    std::vector<std::string> expectedMoveList2{"d5c6"};
    BOOST_TEST(CURRENT_MOVES()->GetMoveList() == expectedMoveList2, boost::test_tools::per_element());

    CURRENT_MOVES()->CleanUp();

    CURRENT_BOARD()->SetBoard("8/8/5k2/8/2pP4/8/8/4K3 b - d3 0 1");
    CURRENT_BOARD()->PrintBoard();
    std::vector<std::string> expectedMoveList3{"c4c3", "c4d3"};
    BOOST_TEST(CURRENT_MOVES()->GetMoveList() == expectedMoveList3, boost::test_tools::per_element());
}
