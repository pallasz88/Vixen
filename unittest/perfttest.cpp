#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test

#include "fixture.h"
#include "move_generator.h"
#include "hash.h"


using namespace Vixen;

BOOST_FIXTURE_TEST_SUITE(Test_perft, Fixture)

    BOOST_AUTO_TEST_CASE(Test_start_position)
    {
        board.PrintBoard();
        BOOST_TEST(MoveGenerator::PerftTest(5, board) == 4865609);
    }

    BOOST_AUTO_TEST_CASE(Test_kiwipete_position)
    {
        board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        board.PrintBoard();
        BOOST_TEST(MoveGenerator::PerftTest(3, board) == 97862);
    }

BOOST_AUTO_TEST_SUITE_END()