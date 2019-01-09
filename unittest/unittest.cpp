#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Vixen_chess

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

BOOST_AUTO_TEST_CASE(Test_bit)
{
    BOOST_TEST(1U == SquareToBitBoard(H1));
    BOOST_TEST(128U == SquareToBitBoard(A1));
    BOOST_TEST(1099511627776U == SquareToBitBoard(H6));

    BOOST_TEST(0 == TrailingZeroCount(SquareToBitBoard(H1)));
    BOOST_TEST(45 == TrailingZeroCount(SquareToBitBoard(C6)));
    BOOST_TEST(33 == TrailingZeroCount(SquareToBitBoard(G5)));
    BOOST_TEST(17 == TrailingZeroCount(4620710852818501632ULL));
}

BOOST_FIXTURE_TEST_SUITE(Test_unit, F)

    BOOST_AUTO_TEST_CASE(Test_Startposition)
    {
        Vixen::Board board;
        auto bitBoards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(bitBoards.at('K'), 8ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('Q'), 16ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('R'), 129ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('B'), 36ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('N'), 66ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('P'), RANK2);

        BOOST_CHECK_EQUAL(bitBoards.at('p'), RANK7);
        BOOST_CHECK_EQUAL(bitBoards.at('n'), 4755801206503243776ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('b'), 2594073385365405696ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('r'), 9295429630892703744ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('q'), 1152921504606846976ULL);
        BOOST_CHECK_EQUAL(bitBoards.at('k'), 576460752303423488ULL);

        BOOST_CHECK_EQUAL(board.GetCastlingRights(), 0xF);

        BOOST_CHECK_EQUAL(board.GetEnPassant(), EMPTY_BOARD);

        BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
    }

    BOOST_AUTO_TEST_CASE(Test_SetPosition)
    {
        board.SetBoard(TESTPOS1);
        board.PrintBoard();

        auto boards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(boards.at('K'), 2ULL);
        BOOST_CHECK_EQUAL(boards.at('Q'), 524288ULL);
        BOOST_CHECK_EQUAL(boards.at('R'), 132ULL);
        BOOST_CHECK_EQUAL(boards.at('B'), 67109376ULL);
        BOOST_CHECK_EQUAL(boards.at('N'), 4096ULL);
        BOOST_CHECK_EQUAL(boards.at('P'), 103081542656ULL);

        BOOST_CHECK_EQUAL(boards.at('p'), 3802523525709824ULL);
        BOOST_CHECK_EQUAL(boards.at('n'), 536870912ULL);
        BOOST_CHECK_EQUAL(boards.at('b'), 5066549580791808ULL);
        BOOST_CHECK_EQUAL(boards.at('r'), 2594073385365405696ULL);
        BOOST_CHECK_EQUAL(boards.at('q'), 1152921504606846976ULL);
        BOOST_CHECK_EQUAL(boards.at('k'), 144115188075855872ULL);

        BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);

        board.SetBoard(TESTPOS2);
        board.PrintBoard();

        boards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(boards.at('K'), 8ULL);
        BOOST_CHECK_EQUAL(boards.at('Q'), 16ULL);
        BOOST_CHECK_EQUAL(boards.at('R'), 129ULL);
        BOOST_CHECK_EQUAL(boards.at('B'), 67108868ULL);
        BOOST_CHECK_EQUAL(boards.at('N'), 66ULL);
        BOOST_CHECK_EQUAL(boards.at('P'), 583424ULL);

        BOOST_CHECK_EQUAL(boards.at('p'), 62768920074911744ULL);
        BOOST_CHECK_EQUAL(boards.at('n'), 4611690416473899008ULL);
        BOOST_CHECK_EQUAL(boards.at('b'), 2594073385365405696ULL);
        BOOST_CHECK_EQUAL(boards.at('r'), 9295429630892703744ULL);
        BOOST_CHECK_EQUAL(boards.at('q'), 1152921504606846976ULL);
        BOOST_CHECK_EQUAL(boards.at('k'), 576460752303423488ULL);

        BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
    }

    BOOST_AUTO_TEST_CASE(Test_SliderAttacks)
    {
        auto bitBoards = board.GetBitBoards();
        auto attacks = board.GetSlider();
        BOOST_CHECK_EQUAL(attacks.GetRookAttack(D4, ~bitBoards.at(' ')), 4521264543698944ULL);
        BOOST_CHECK_EQUAL(attacks.GetBishopAttack(D4, ~bitBoards.at(' ')), 36666685564404736ULL);
        BOOST_CHECK_EQUAL(attacks.GetQueenAttack(D4, ~bitBoards.at(' ')), 41187950108103680ULL);

        BOOST_CHECK_EQUAL(attacks.GetRookAttack(H1, ~bitBoards.at(' ')), 258ULL);
        BOOST_CHECK_EQUAL(attacks.GetBishopAttack(H1, ~bitBoards.at(' ')), 512ULL);
        BOOST_CHECK_EQUAL(attacks.GetQueenAttack(H1, ~bitBoards.at(' ')), 770ULL);

        BOOST_CHECK_EQUAL(attacks.GetRookAttack(D8, ~bitBoards.at(' ')), 2886807361144487936ULL);
        BOOST_CHECK_EQUAL(attacks.GetBishopAttack(D8, ~bitBoards.at(' ')), 11258999068426240ULL);
        BOOST_CHECK_EQUAL(attacks.GetQueenAttack(D8, ~bitBoards.at(' ')), 2898066360212914176ULL);

        board.SetBoard(TESTPOS2);
        bitBoards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(attacks.GetRookAttack(D4, ~bitBoards.at(' ')), 4521264493367312ULL);
        BOOST_CHECK_EQUAL(attacks.GetBishopAttack(D4, ~bitBoards.at(' ')), 36103735610982400ULL);
        BOOST_CHECK_EQUAL(attacks.GetQueenAttack(D4, ~bitBoards.at(' ')), 40625000104349712ULL);
    }

    BOOST_AUTO_TEST_CASE(Test_pawnmovement)
    {
        board.SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{2332};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList, boost::test_tools::per_element());

        board.SetBoard("8/4p3/2npP3/3P4/8/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{19300};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList2, boost::test_tools::per_element());

        board.SetBoard("8/8/8/8/2pP4/8/8/8 b - d3 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{21789, 1373};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList3, boost::test_tools::per_element());

        board.SetBoard("8/8/8/7p/p6P/P6p/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4;
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList4, boost::test_tools::per_element());

        board.SetBoard("8/8/8/7p/p6P/P6p/7P/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList5;
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList5, boost::test_tools::per_element());

        board.SetBoard("8/2P2P2/8/8/8/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList6{48818, 44722, 40626, 36530, 49013, 44917, 40821, 36725};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList6, boost::test_tools::per_element());

        board.SetBoard("1n1qb1r1/2P2P2/8/8/8/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList7{65266, 61170, 57074, 52978,
                                            65461, 61365, 57269, 53173,
                                            65138, 61042, 56946, 52850,
                                            65333, 61237, 57141, 53045,
                                            48818, 44722, 40626, 36530,
                                            49013, 44917, 40821, 36725};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList7, boost::test_tools::per_element());
    }

    BOOST_AUTO_TEST_CASE(Test_knightmovement)
    {
        board.SetBoard("8/8/8/8/3N4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{732, 860, 1180, 1436, 2204, 2460, 2780, 2908};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList, boost::test_tools::per_element());

        board.SetBoard("N6N/8/8/1N4N1/8/8/8/N6N w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{640, 1088,
                                            839, 1415,
                                            1057, 1185, 1761, 2785, 3105, 3233,
                                            1382, 1510, 1830, 2854, 3430, 3558,
                                            2680, 3256,
                                            3007, 3455};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList2, boost::test_tools::per_element());

        board.SetBoard("8/8/8/8/3n4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{732, 860, 1180, 1436, 2204, 2460, 2780, 2908};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList3, boost::test_tools::per_element());

        board.SetBoard("n6n/8/8/1n4n1/8/6B1/5QK1/n6n b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4{17024, 17472,
                                            839, 1415,
                                            1057, 1185, 1761, 2785, 3105, 3233,
                                            1382, 1510, 1830, 2854, 3430, 3558,
                                            2680, 3256,
                                            3007, 3455};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList4, boost::test_tools::per_element());
    }

    BOOST_AUTO_TEST_CASE(Test_bishopmovement)
    {
        board.SetBoard("8/8/8/8/3B4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                           2972, 3164, 3548, 3612};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList, boost::test_tools::per_element());

        board.SetBoard("8/Bp4pB/1P4P1/8/8/1p4p1/BP4PB/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{17480, 17807, 72, 399, 3696, 4023};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList2, boost::test_tools::per_element());

        board.SetBoard("8/8/8/8/3b4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                            2972, 3164, 3548, 3612};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList3, boost::test_tools::per_element());

        board.SetBoard("8/bp4pb/1P4P1/8/8/1p4p1/bP4Pb/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4{19056, 19383, 72, 399, 3696, 4023};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList4, boost::test_tools::per_element());
    }

    BOOST_AUTO_TEST_CASE(Test_rookmovement)
    {
        board.SetBoard("8/8/8/8/3R4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{284, 796, 1308, 1564, 1628, 1692, 1756, 1884, 1948,
                                           2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList, boost::test_tools::per_element());

        board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{64, 128, 192, 256, 320};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList2, boost::test_tools::per_element());


        board.SetBoard("8/8/8/8/3r4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{284, 796, 1308, 1564, 1628, 1692, 1756, 1884, 1948,
                                            2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList3, boost::test_tools::per_element());

        board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4{18943, 3071, 3583, 4031};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList4, boost::test_tools::per_element());
    }

    BOOST_AUTO_TEST_CASE(Test_queenmovement)
    {
        board.SetBoard("8/8/8/8/3Q4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                           2972, 3164, 3548, 3612, 284, 796, 1308, 1564, 1628,
                                           1692, 1756, 1884, 1948, 2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList, boost::test_tools::per_element());

        board.SetBoard("8/8/8/8/3q4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                            2972, 3164, 3548, 3612, 284, 796, 1308, 1564, 1628,
                                            1692, 1756, 1884, 1948, 2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList2, boost::test_tools::per_element());
    }

    BOOST_AUTO_TEST_CASE(Test_kingmovement)
    {
        board.SetBoard("8/8/8/8/3K4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{1244, 1308, 1372, 1756, 1884, 2268, 2332, 2396};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList, boost::test_tools::per_element());

        board.SetBoard("8/8/8/8/3k4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{1244, 1308, 1372, 1756, 1884, 2268, 2332, 2396};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList2, boost::test_tools::per_element());

        board.SetBoard("r3k2r/8/8/8/8/p6p/P6P/R3K2R w KQkq - 0 52");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{131, 259, 643, 707, 771, 64, 128, 263, 327, 391, 8259, 12611};
        BOOST_TEST(MoveGenerator::GetAllMoves(board) == expectedMoveList3, boost::test_tools::per_element());
    }

    BOOST_AUTO_TEST_CASE(Test_huge_amunt_of_moves)
    {
        board.SetBoard("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1");
        board.PrintBoard();
        BOOST_TEST(MoveGenerator::GetAllMoves(board).size() == 218);

        board.SetBoard("3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1");
        board.PrintBoard();
        BOOST_TEST(MoveGenerator::GetAllMoves(board).size() == 218);

        board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        board.PrintBoard();
        BOOST_TEST(MoveGenerator::GetAllMoves(board).size() == 48);
    }

    BOOST_AUTO_TEST_CASE(Test_making_moves)
    {
        board.SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
        board.MakeMove(2332);
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoards().at('P') == 68719476736ULL);

        board.SetBoard("1n1qb1r1/2P2P2/8/8/8/8/8/8 w - - 0 1");
        BOOST_TEST(board.MakeMove(65266));
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoards().at('P') == 9007199254740992ULL);
        BOOST_TEST(board.GetBitBoards().at('Q') == 576460752303423488ULL);
        BOOST_TEST(board.GetBitBoards().at('b') == EMPTY_BOARD);
        BOOST_TEST(board.GetBitBoards().at(' ') == 11952553411041296383ULL);
        BOOST_TEST(board.GetBitBoards().at('F') == 585467951558164480ULL);
        BOOST_TEST(board.GetBitBoards().at('S') == 5908722711110090752ULL);

        board.SetBoard("r3k2r/8/8/8/8/p6p/P6P/R3K2R w KQkq - 0 1");
        BOOST_TEST(board.MakeMove(8259));
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoards().at('K') == 2ULL);
        BOOST_TEST(board.GetBitBoards().at('R') == 132ULL);
        BOOST_TEST(board.GetBitBoards().at(' ') == 8574853690504937081ULL);
        BOOST_TEST(board.GetBitBoards().at('F') == 33158ULL);
        BOOST_TEST(board.GetBitBoards().at('S') == 9871890383204581376ULL);
        BOOST_TEST(board.GetCastlingRights() == 0b0011);

        board.SetBoard("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 b - - 0 1");
        BOOST_TEST(!board.MakeMove(16775));
        board.PrintBoard();
    }

BOOST_AUTO_TEST_SUITE_END()