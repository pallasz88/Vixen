#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_mirror_position

#include "fixture.h"
#include "move_generator.hpp"
#include <sstream>

BOOST_FIXTURE_TEST_SUITE(Test_perft, vixen::Fixture)

BOOST_AUTO_TEST_CASE(Test_mirror_position)
{
    board.SetBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    BOOST_TEST(vixen::Test::PerftTest(5, board) == 15833292);
    board.PrintBoard();

    board.SetBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    BOOST_TEST(vixen::Test::PerftTest(5, board) == 15833292);
    board.PrintBoard();
}

BOOST_AUTO_TEST_SUITE_END()