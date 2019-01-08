#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test

#include <boost/test/included/unit_test.hpp>
#include "board.h"
#include "hash.h"
#include "move_generator.h"

using namespace Vixen;

BOOST_AUTO_TEST_CASE(Test_start_position)
{
    Board board;
    //board.SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    board.PrintBoard();
    Timer t("perft");
    BOOST_TEST(MoveGenerator::PerftTest(5, board) == 4865609);
}