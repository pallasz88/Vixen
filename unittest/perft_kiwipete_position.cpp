#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test_kiwipete_position

#include "fixture.h"
#include "hash.hpp"
#include "move_generator.hpp"
#include <sstream>

vixen::Hash::Keys vixen::Hash::zobristKeys;

BOOST_FIXTURE_TEST_SUITE(Test_perft, vixen::Fixture)

BOOST_AUTO_TEST_CASE(Test_kiwipete_position)
{
    board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    BOOST_TEST(vixen::Test::PerftTest(4, board) == 4085603);
    board.PrintBoard();
}

BOOST_AUTO_TEST_SUITE_END()