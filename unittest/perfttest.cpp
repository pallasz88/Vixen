#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test

#include <boost/test/included/unit_test.hpp>
#include "board.h"
#include "move_generator.h"
#include "hash.h"

using namespace Vixen;

struct F
{
    F() : board(Board())
    {
        BOOST_TEST_MESSAGE("setup fixture");
    }

    ~F()
    { BOOST_TEST_MESSAGE("teardown fixture"); }

    Board board;
};

BOOST_FIXTURE_TEST_SUITE(Test_perft, F)

    BOOST_AUTO_TEST_CASE(Test_start_position)
    {
        board.PrintBoard();
        BOOST_TEST(MoveGenerator::PerftTest(4, board) == 197281);
    }

    BOOST_AUTO_TEST_CASE(Test_kiwipete_position)
    {
        board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        board.PrintBoard();
        BOOST_TEST(MoveGenerator::PerftTest(2, board) == 2039);
    }

BOOST_AUTO_TEST_SUITE_END()