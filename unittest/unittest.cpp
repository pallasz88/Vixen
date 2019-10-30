#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Vixen_chess

#include "fixture.h"
#include "move_generator.h"
#include "hash.h"
#include "userinterface.h"


using namespace Vixen;

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

BOOST_FIXTURE_TEST_SUITE(Test_unit, Fixture)

    BOOST_AUTO_TEST_CASE(Test_Startposition)
    {
        Vixen::Board board;
        BOOST_CHECK_EQUAL(board.GetBitBoard('K'), 8ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('Q'), 16ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('R'), 129ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('B'), 36ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('N'), 66ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('P'), RANK2);

        BOOST_CHECK_EQUAL(board.GetBitBoard('p'), RANK7);
        BOOST_CHECK_EQUAL(board.GetBitBoard('n'), 4755801206503243776ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('b'), 2594073385365405696ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('r'), 9295429630892703744ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('q'), 1152921504606846976ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('k'), 576460752303423488ULL);

        BOOST_CHECK_EQUAL(board.GetCastlingRights(), 0xF);

        BOOST_CHECK_EQUAL(board.GetEnPassant(), EMPTY_BOARD);

        BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
    }

    BOOST_AUTO_TEST_CASE(Test_SetPosition)
    {
        board.SetBoard(TESTPOS1);
        board.PrintBoard();

        BOOST_CHECK_EQUAL(board.GetBitBoard('K'), 2ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('Q'), 524288ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('R'), 132ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('B'), 67109376ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('N'), 4096ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('P'), 103081542656ULL);

        BOOST_CHECK_EQUAL(board.GetBitBoard('p'), 3802523525709824ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('n'), 536870912ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('b'), 5066549580791808ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('r'), 2594073385365405696ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('q'), 1152921504606846976ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('k'), 144115188075855872ULL);

        BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);

        board.SetBoard(TESTPOS2);
        board.PrintBoard();

        BOOST_CHECK_EQUAL(board.GetBitBoard('K'), 8ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('Q'), 16ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('R'), 129ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('B'), 67108868ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('N'), 66ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('P'), 583424ULL);

        BOOST_CHECK_EQUAL(board.GetBitBoard('p'), 62768920074911744ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('n'), 4611690416473899008ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('b'), 2594073385365405696ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('r'), 9295429630892703744ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('q'), 1152921504606846976ULL);
        BOOST_CHECK_EQUAL(board.GetBitBoard('k'), 576460752303423488ULL);

        BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
    }
/*
    BOOST_AUTO_TEST_CASE(Test_SliderAttacks)
    {
        auto bitBoards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(GetRookAttack(D4, ~board.GetBitBoard(' ')), 4521264543698944ULL);
        BOOST_CHECK_EQUAL(GetBishopAttack(D4, ~board.GetBitBoard(' ')), 36666685564404736ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(D4, ~board.GetBitBoard(' ')), 41187950108103680ULL);

        BOOST_CHECK_EQUAL(GetRookAttack(H1, ~board.GetBitBoard(' ')), 258ULL);
        BOOST_CHECK_EQUAL(GetBishopAttack(H1, ~board.GetBitBoard(' ')), 512ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(H1, ~board.GetBitBoard(' ')), 770ULL);

        BOOST_CHECK_EQUAL(GetRookAttack(D8, ~board.GetBitBoard(' ')), 2886807361144487936ULL);
        BOOST_CHECK_EQUAL(GetBishopAttack(D8, ~board.GetBitBoard(' ')), 11258999068426240ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(D8, ~board.GetBitBoard(' ')), 2898066360212914176ULL);

        board.SetBoard(TESTPOS2);
        bitBoards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(GetRookAttack(D4, ~board.GetBitBoard(' ')), 4521264493367312ULL);
        BOOST_CHECK_EQUAL(GetBishopAttack(D4, ~board.GetBitBoard(' ')), 36103735610982400ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(D4, ~board.GetBitBoard(' ')), 40625000104349712ULL);
    }
*/
    BOOST_AUTO_TEST_CASE(Test_pawnmovement)
    {
        board.SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{2332};
        BOOST_TEST(CheckMoveList(expectedMoveList));

        board.SetBoard("8/4p3/2npP3/3P4/8/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{19300};
        BOOST_TEST(CheckMoveList(expectedMoveList2));

        board.SetBoard("8/8/8/8/2pP4/8/8/8 b - d3 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{21789, 1373};
        BOOST_TEST(CheckMoveList(expectedMoveList3));

        board.SetBoard("8/8/8/7p/p6P/P6p/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4;
        BOOST_TEST(CheckMoveList(expectedMoveList4));

        board.SetBoard("8/8/8/7p/p6P/P6p/7P/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList5;
        BOOST_TEST(CheckMoveList(expectedMoveList5));

        board.SetBoard("8/2P2P2/8/8/8/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList6{48818, 44722, 40626, 36530, 49013, 44917, 40821, 36725};
        BOOST_TEST(CheckMoveList(expectedMoveList6));

        board.SetBoard("1n1qb1r1/2P2P2/8/8/8/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList7{65266, 61170, 57074, 52978,
                                            65461, 61365, 57269, 53173,
                                            65138, 61042, 56946, 52850,
                                            65333, 61237, 57141, 53045,
                                            48818, 44722, 40626, 36530,
                                            49013, 44917, 40821, 36725};
        BOOST_TEST(CheckMoveList(expectedMoveList7));
    }

    BOOST_AUTO_TEST_CASE(Test_knightmovement)
    {
        board.SetBoard("8/8/8/8/3N4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{732, 860, 1180, 1436, 2204, 2460, 2780, 2908};
        BOOST_TEST(CheckMoveList(expectedMoveList));

        board.SetBoard("N6N/8/8/1N4N1/8/8/8/N6N w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{640, 1088,
                                            839, 1415,
                                            1057, 1185, 1761, 2785, 3105, 3233,
                                            1382, 1510, 1830, 2854, 3430, 3558,
                                            2680, 3256,
                                            3007, 3455};
        BOOST_TEST(CheckMoveList(expectedMoveList2));

        board.SetBoard("8/8/8/8/3n4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{732, 860, 1180, 1436, 2204, 2460, 2780, 2908};
        BOOST_TEST(CheckMoveList(expectedMoveList3));

        board.SetBoard("n6n/8/8/1n4n1/8/6B1/5QK1/n6n b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4{17024, 17472,
                                            839, 1415,
                                            1057, 1185, 1761, 2785, 3105, 3233,
                                            1382, 1510, 1830, 2854, 3430, 3558,
                                            2680, 3256,
                                            3007, 3455};
        BOOST_TEST(CheckMoveList(expectedMoveList4));
    }

    BOOST_AUTO_TEST_CASE(Test_bishopmovement)
    {
        board.SetBoard("8/8/8/8/3B4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                           2972, 3164, 3548, 3612};
        BOOST_TEST(CheckMoveList(expectedMoveList));

        board.SetBoard("8/Bp4pB/1P4P1/8/8/1p4p1/BP4PB/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{17480, 17807, 72, 399, 3696, 4023};
        BOOST_TEST(CheckMoveList(expectedMoveList2));

        board.SetBoard("8/8/8/8/3b4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                            2972, 3164, 3548, 3612};
        BOOST_TEST(CheckMoveList(expectedMoveList3));

        board.SetBoard("8/bp4pb/1P4P1/8/8/1p4p1/bP4Pb/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4{19056, 19383, 72, 399, 3696, 4023};
        BOOST_TEST(CheckMoveList(expectedMoveList4));
    }

    BOOST_AUTO_TEST_CASE(Test_rookmovement)
    {
        board.SetBoard("8/8/8/8/3R4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{284, 796, 1308, 1564, 1628, 1692, 1756, 1884, 1948,
                                           2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(CheckMoveList(expectedMoveList));

        board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{64, 128, 192, 256, 320};
        BOOST_TEST(CheckMoveList(expectedMoveList2));


        board.SetBoard("8/8/8/8/3r4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{284, 796, 1308, 1564, 1628, 1692, 1756, 1884, 1948,
                                            2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(CheckMoveList(expectedMoveList3));

        board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList4{18943, 3071, 3583, 4031};
        BOOST_TEST(CheckMoveList(expectedMoveList4));
    }

    BOOST_AUTO_TEST_CASE(Test_queenmovement)
    {
        board.SetBoard("8/8/8/8/3Q4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                           2972, 3164, 3548, 3612, 284, 796, 1308, 1564, 1628,
                                           1692, 1756, 1884, 1948, 2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(CheckMoveList(expectedMoveList));

        board.SetBoard("8/8/8/8/3q4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{92, 476, 668, 924, 1244, 1372, 2268, 2396, 2716,
                                            2972, 3164, 3548, 3612, 284, 796, 1308, 1564, 1628,
                                            1692, 1756, 1884, 1948, 2012, 2332, 2844, 3356, 3868};
        BOOST_TEST(CheckMoveList(expectedMoveList2));
    }

    BOOST_AUTO_TEST_CASE(Test_kingmovement)
    {
        board.SetBoard("8/8/8/8/3K4/8/8/8 w - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList{1244, 1308, 1372, 1756, 1884, 2268, 2332, 2396};
        BOOST_TEST(CheckMoveList(expectedMoveList));

        board.SetBoard("8/8/8/8/3k4/8/8/8 b - - 0 1");
        board.PrintBoard();
        std::vector<Move> expectedMoveList2{1244, 1308, 1372, 1756, 1884, 2268, 2332, 2396};
        BOOST_TEST(CheckMoveList(expectedMoveList2));

        board.SetBoard("r3k2r/8/8/8/8/p6p/P6P/R3K2R w KQkq - 0 52");
        board.PrintBoard();
        std::vector<Move> expectedMoveList3{131, 259, 643, 707, 771, 64, 128, 263, 327, 391, 8259, 12611};
        BOOST_TEST(CheckMoveList(expectedMoveList3));
    }

    BOOST_AUTO_TEST_CASE(Test_huge_amunt_of_moves)
    {
        board.SetBoard("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1");
        board.PrintBoard();
        BOOST_TEST(board.CreateGenerator<ALL_MOVE>().GetListSize() == 218);

        board.SetBoard("3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1");
        board.PrintBoard();
        BOOST_TEST(board.CreateGenerator<ALL_MOVE>().GetListSize() == 218);

        board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        board.PrintBoard();
        BOOST_TEST(board.CreateGenerator<ALL_MOVE>().GetListSize() == 48);
    }

    BOOST_AUTO_TEST_CASE(Test_making_moves)
    {
        board.SetBoard("7k/8/8/8/3P4/8/8/7K w - - 0 1");
        board.MakeMove(2332);
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('P') == 68719476736ULL);

        board.SetBoard("1n1qb1r1/2P2P2/8/k7/8/8/8/4K3 w - - 0 1");
        BOOST_TEST(board.MakeMove(65266));
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('P') == 9007199254740992ULL);
        BOOST_TEST(board.GetBitBoard('Q') == 576460752303423488ULL);
        BOOST_TEST(board.GetBitBoard('b') == EMPTY_BOARD);
        BOOST_TEST(board.GetBitBoard(' ') == 11952552861285482487ULL);
        BOOST_TEST(board.GetBitBoard('F') == 585467951558164488ULL);
        BOOST_TEST(board.GetBitBoard('S') == 5908723260865904640ULL);

        board.TakeBack();
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('P') == 10133099161583616ULL);
        BOOST_TEST(board.GetBitBoard('Q') == EMPTY_BOARD);
        BOOST_TEST(board.GetBitBoard('b') == 576460752303423488ULL);
        BOOST_TEST(board.GetBitBoard(' ') == 11951426961378639863ULL);
        BOOST_TEST(board.GetBitBoard('F') == 10133099161583624ULL);
        BOOST_TEST(board.GetBitBoard('S') == 6485184013169328128ULL);

        board.SetBoard("r3k2r/8/8/8/8/p6p/P6P/R3K2R w KQkq - 0 1");
        BOOST_TEST(board.MakeMove(8259));
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('K') == 2ULL);
        BOOST_TEST(board.GetBitBoard('R') == 132ULL);
        BOOST_TEST(board.GetBitBoard(' ') == 8574853690504937081ULL);
        BOOST_TEST(board.GetBitBoard('F') == 33158ULL);
        BOOST_TEST(board.GetBitBoard('S') == 9871890383204581376ULL);
        BOOST_TEST(board.GetCastlingRights() == 0b0011);

        board.SetBoard("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 b - - 0 1");
        BOOST_TEST(!board.MakeMove(16775));
        board.PrintBoard();

        // CASTLING
        board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        BOOST_TEST(board.MakeMove(8259));
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('K') == 2ULL);
        BOOST_TEST(board.GetBitBoard('R') == 132ULL);
        BOOST_TEST(board.GetBitBoard(' ') == 8521146841253412985ULL);
        BOOST_TEST(board.GetBitBoard('F') == 103215857542ULL);
        BOOST_TEST(board.GetBitBoard('S') == 9925597129240281088ULL);
        BOOST_TEST(board.GetCastlingRights() == 0b0011);

        board.TakeBack();
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('K') == 8ULL);
        BOOST_TEST(board.GetBitBoard('R') == 129ULL);
        BOOST_TEST(board.GetBitBoard(' ') == 8521146841253412982ULL);
        BOOST_TEST(board.GetBitBoard('F') == 103215857545ULL);
        BOOST_TEST(board.GetBitBoard('S') == 9925597129240281088ULL);
        BOOST_TEST(board.GetCastlingRights() == 0b1111);

        // ENPASSANT
        board.MakeMove(6095);
        board.MakeMove(21982);
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('P') == 68853720832ULL);
        BOOST_TEST(board.GetBitBoard('p') == 50676490932649984ULL);
        BOOST_TEST(board.GetBitBoard(' ') == 8521146842318798966ULL);
        BOOST_TEST(board.GetBitBoard('F') == 103215824777ULL);
        BOOST_TEST(board.GetBitBoard('S') == 9925597128174927872ULL);

        board.TakeBack();
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('P') == 71001204480ULL);
        BOOST_TEST(board.GetBitBoard('p') == 50676491998003200ULL);
        BOOST_TEST(board.GetBitBoard(' ') == 8521146839105962102ULL);
        BOOST_TEST(board.GetBitBoard('F') == 105363308425ULL);
        BOOST_TEST(board.GetBitBoard('S') == 9925597129240281088ULL);

        board.TakeBack();
        board.MakeMove(17417);
        board.MakeMove(3771);
        board.PrintBoard();
        BOOST_TEST(Test::PerftTest(2, board) == 1790);
        board.TakeBack();
        board.TakeBack();
        board.PrintBoard();
    }

    BOOST_AUTO_TEST_CASE(Test_userinterface)
    {
        board.SetBoard("7k/8/8/8/3P4/8/8/7K w - - 0 1");
        UserInterface::PrintMoveList(board);
        UserInterface::MakeMove("d4d5", board);
        UserInterface::TakeBackMove(board);
        board.PrintBoard();
        BOOST_TEST(board.GetBitBoard('P') == 268435456ULL);
    }

BOOST_AUTO_TEST_SUITE_END()
