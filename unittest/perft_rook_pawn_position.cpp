#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test_rook_pawn_position

#include "fixture.h"
#include "hash.hpp"
#include "move_generator.hpp"
#include <sstream>

vixen::Hash::Keys vixen::Hash::zobristKeys;

BOOST_FIXTURE_TEST_SUITE(Test_perft, vixen::Fixture)

BOOST_AUTO_TEST_CASE(Test_rook_pawn)
{
    board.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    BOOST_TEST(vixen::Test::PerftTest(6, board) == 11030083);
    board.PrintBoard();
}

BOOST_AUTO_TEST_SUITE_END()