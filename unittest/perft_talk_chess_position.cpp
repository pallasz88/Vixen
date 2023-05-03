#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test_talk_chess_position

#include "fixture.h"
#include "hash.hpp"
#include "move_generator.hpp"
#include <sstream>

vixen::Hash::Keys vixen::Hash::zobristKeys;

BOOST_FIXTURE_TEST_SUITE(Test_perft, vixen::Fixture)

BOOST_AUTO_TEST_CASE(Test_talk_chess)
{
    board.SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    BOOST_TEST(vixen::Test::PerftTest(5, board) == 89941194);
    board.PrintBoard();
}

BOOST_AUTO_TEST_SUITE_END()