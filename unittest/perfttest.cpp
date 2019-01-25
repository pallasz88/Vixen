#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Perft_test

#include "fixture.h"
#include "move_generator.h"
#include "hash.h"


using namespace Vixen;

BOOST_FIXTURE_TEST_SUITE(Test_perft, Fixture)

    BOOST_AUTO_TEST_CASE(Test_start_position)
    {
        BOOST_TEST(MoveGenerator::PerftTest(5, board) == 4865609);
        board.PrintBoard();
    }

    BOOST_AUTO_TEST_CASE(Test_kiwipete_position)
    {
        board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        BOOST_TEST(MoveGenerator::PerftTest(4, board) == 4085603);
        board.PrintBoard();
    }

    BOOST_AUTO_TEST_CASE(Test_rook_pawn)
    {
        board.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        BOOST_TEST(MoveGenerator::PerftTest(6, board) == 11030083);
        board.PrintBoard();
    }

    BOOST_AUTO_TEST_CASE(Test_talk_chess)
    {
        board.SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
        BOOST_TEST(MoveGenerator::PerftTest(4, board) == 2103487);
        board.PrintBoard();
    }

BOOST_AUTO_TEST_SUITE_END()