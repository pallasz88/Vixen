#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test_alternative_position

#include "fixture.h"
#include "move_generator.hpp"
#include <sstream>

BOOST_FIXTURE_TEST_SUITE(Test_perft, vixen::Fixture)

BOOST_AUTO_TEST_CASE(Test_alternative_chess)
{
    board.SetBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
                   "10");
    BOOST_TEST(vixen::Test::PerftTest(4, board) == 3894594);
    board.PrintBoard();
}

BOOST_AUTO_TEST_SUITE_END()