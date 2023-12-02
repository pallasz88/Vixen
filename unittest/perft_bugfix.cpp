#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test_start_position

#include "fixture.h"
#include "hash.hpp"
#include "move_generator.hpp"
#include <sstream>

vixen::Hash::Keys vixen::Hash::zobristKeys;

BOOST_FIXTURE_TEST_SUITE(Test_perft, vixen::Fixture)

BOOST_AUTO_TEST_CASE(Test_start_position)
{
    board.SetBoard("8/8/8/4b3/8/8/3k1K1p/8 b - - 21 126");
    BOOST_TEST(vixen::Test::PerftTest(5, board) == 145147);
    board.PrintBoard();
}

BOOST_AUTO_TEST_SUITE_END()