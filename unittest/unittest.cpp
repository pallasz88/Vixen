#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Vixen_chess

#include "defs.hpp"
#include "engine.hpp"
#include "fixture.h"
#include "hash.hpp"
#include "move_generator.hpp"
#include "userinterface.hpp"

vixen::Hash::Keys vixen::Hash::zobristKeys;

BOOST_AUTO_TEST_CASE(Test_bit)
{
    using namespace vixen;
    BOOST_TEST(1U == SquareToBitBoard(static_cast<unsigned>(Squares::H1)));
    BOOST_TEST(128U == SquareToBitBoard(static_cast<unsigned>(Squares::A1)));
    BOOST_TEST(1099511627776U == SquareToBitBoard(static_cast<unsigned>(Squares::H6)));

    BOOST_TEST(0 == TrailingZeroCount(SquareToBitBoard(static_cast<unsigned>(Squares::H1))));
    BOOST_TEST(45 == TrailingZeroCount(SquareToBitBoard(static_cast<unsigned>(Squares::C6))));
    BOOST_TEST(33 == TrailingZeroCount(SquareToBitBoard(static_cast<unsigned>(Squares::G5))));
    BOOST_TEST(17 == TrailingZeroCount(4620710852818501632ULL));
}

BOOST_FIXTURE_TEST_SUITE(Test_unit, vixen::Fixture)

BOOST_AUTO_TEST_CASE(Test_Startposition)
{
    using namespace vixen;
    Board board;
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_KING_INDEX), 8ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_QUEEN_INDEX), 16ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_ROOK_INDEX), 129ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_BISHOP_INDEX), 36ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_KNIGHT_INDEX), 66ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_PAWN_INDEX), static_cast<BitBoard>(Ranks::RANK2));

    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_PAWN_INDEX), static_cast<BitBoard>(Ranks::RANK7));
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_KNIGHT_INDEX), 4755801206503243776ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_BISHOP_INDEX), 2594073385365405696ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_ROOK_INDEX), 9295429630892703744ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_QUEEN_INDEX), 1152921504606846976ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_KING_INDEX), 576460752303423488ULL);

    BOOST_CHECK_EQUAL(board.GetCastlingRights(), 0xF);

    BOOST_CHECK_EQUAL(board.GetEnPassant(), Constants::EMPTY_BOARD);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
}

BOOST_AUTO_TEST_CASE(Test_SetPosition)
{
    using namespace vixen;
    board.SetBoard(Constants::TESTPOS1);
    board.PrintBoard();

    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_KING_INDEX), 2ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_QUEEN_INDEX), 524288ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_ROOK_INDEX), 132ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_BISHOP_INDEX), 67109376ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_KNIGHT_INDEX), 4096ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_PAWN_INDEX), 103081542656ULL);

    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_PAWN_INDEX), 3802523525709824ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_KNIGHT_INDEX), 536870912ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_BISHOP_INDEX), 5066549580791808ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_ROOK_INDEX), 2594073385365405696ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_QUEEN_INDEX), 1152921504606846976ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_KING_INDEX), 144115188075855872ULL);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);

    board.SetBoard(Constants::TESTPOS2);
    board.PrintBoard();

    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_KING_INDEX), 8ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_QUEEN_INDEX), 16ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_ROOK_INDEX), 129ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_BISHOP_INDEX), 67108868ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_KNIGHT_INDEX), 66ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::WHITE_PAWN_INDEX), 583424ULL);

    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_PAWN_INDEX), 62768920074911744ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_KNIGHT_INDEX), 4611690416473899008ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_BISHOP_INDEX), 2594073385365405696ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_ROOK_INDEX), 9295429630892703744ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_QUEEN_INDEX), 1152921504606846976ULL);
    BOOST_CHECK_EQUAL(board.GetBitBoard(Constants::BLACK_KING_INDEX), 576460752303423488ULL);

    BOOST_CHECK_EQUAL(board.IsWhiteToMove(), true);
}

BOOST_AUTO_TEST_CASE(Test_HeavyPiecesBitboard)
{
    using namespace vixen;
    board.SetBoard("8/8/8/8/8/5k2/4p3/4K3 w - - 0 4");
    board.PrintBoard();

    BOOST_CHECK_EQUAL(board.HasHeavyPieces(), false);

    board.SetBoard(Constants::TESTPOS2);
    board.PrintBoard();

    BOOST_CHECK_EQUAL(board.HasHeavyPieces(), true);
}
/*
    BOOST_AUTO_TEST_CASE(Test_SliderAttacks)
    {
        auto bitBoards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(GetRookAttack(D4, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)),
   4521264543698944ULL); BOOST_CHECK_EQUAL(GetBishopAttack(D4,
   ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)), 36666685564404736ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(D4, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)),
   41187950108103680ULL);

        BOOST_CHECK_EQUAL(GetRookAttack(H1, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)), 258ULL);
        BOOST_CHECK_EQUAL(GetBishopAttack(H1, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)), 512ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(H1, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)), 770ULL);

        BOOST_CHECK_EQUAL(GetRookAttack(D8, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)),
   2886807361144487936ULL); BOOST_CHECK_EQUAL(GetBishopAttack(D8,
   ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)), 11258999068426240ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(D8, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)),
   2898066360212914176ULL);

        board.SetBoard(TESTPOS2);
        bitBoards = board.GetBitBoards();
        BOOST_CHECK_EQUAL(GetRookAttack(D4, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)),
   4521264493367312ULL); BOOST_CHECK_EQUAL(GetBishopAttack(D4,
   ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)), 36103735610982400ULL);
        BOOST_CHECK_EQUAL(GetQueenAttack(D4, ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX)),
   40625000104349712ULL);
    }
*/

BOOST_AUTO_TEST_CASE(Test_GetMoveList_AllWhiteMoves)
{
    using namespace vixen;
    // Set up the board with a simple starting position
    board.SetBoard(Constants::START_POSITION);

    // Get the list of all moves
    const auto moveList = board.GetMoveList<MoveTypes::ALL_MOVE>();

    // Check that the correct number of moves is generated
    BOOST_CHECK_EQUAL(moveList.size(), 20);
}

BOOST_AUTO_TEST_CASE(Test_GetMoveList_AllBlackMoves)
{
    using namespace vixen;
    // Set up the board with a position where it's black's turn
    board.SetBoard(Constants::TESTPOS3);

    // Get the list of all moves
    const auto moveList = board.GetMoveList<MoveTypes::ALL_MOVE>();

    // Check that the correct number of pseudo moves is generated
    BOOST_CHECK_EQUAL(moveList.size(), 46);
}

BOOST_AUTO_TEST_CASE(Test_Takeback)
{
    using namespace vixen;
    // Set up the board with a simple starting position
    board.SetBoard(Constants::START_POSITION);

    // Take back move. Since we do not have moves we expect throw
    BOOST_REQUIRE_THROW(board.TakeBack(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Test_pawnmovement)
{
    using namespace vixen;
    board.SetBoard("8/8/8/8/3P4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList{2332};
    BOOST_TEST(CheckMoveList(expectedMoveList));

    board.SetBoard("8/4p3/2npP3/3P4/8/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList2{19300};
    BOOST_TEST(CheckMoveList(expectedMoveList2));

    board.SetBoard("8/8/8/8/2pP4/8/8/8 b - d3 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList3{21789, 1373};
    BOOST_TEST(CheckMoveList(expectedMoveList3));

    board.SetBoard("8/8/8/7p/p6P/P6p/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList4;
    BOOST_TEST(CheckMoveList(expectedMoveList4));

    board.SetBoard("8/8/8/7p/p6P/P6p/7P/8 b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList5;
    BOOST_TEST(CheckMoveList(expectedMoveList5));

    board.SetBoard("8/2P2P2/8/8/8/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList6{48818, 44722, 40626, 36530, 49013, 44917, 40821, 36725};
    BOOST_TEST(CheckMoveList(expectedMoveList6));

    board.SetBoard("1n1qb1r1/2P2P2/8/8/8/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList7{65266, 61170, 57074, 52978, 65461, 61365, 57269, 53173,
                                                  65138, 61042, 56946, 52850, 65333, 61237, 57141, 53045,
                                                  48818, 44722, 40626, 36530, 49013, 44917, 40821, 36725};
    BOOST_TEST(CheckMoveList(expectedMoveList7));
}

BOOST_AUTO_TEST_CASE(Test_knightmovement)
{
    using namespace vixen;
    board.SetBoard("8/8/8/8/3N4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList{732, 860, 1180, 1436, 2204, 2460, 2780, 2908};
    BOOST_TEST(CheckMoveList(expectedMoveList));

    board.SetBoard("N6N/8/8/1N4N1/8/8/8/N6N w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList2{640,  1088, 839,  1415, 1057, 1185, 1761, 2785, 3105, 3233,
                                                  1382, 1510, 1830, 2854, 3430, 3558, 2680, 3256, 3007, 3455};
    BOOST_TEST(CheckMoveList(expectedMoveList2));

    board.SetBoard("8/8/8/8/3n4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList3{732, 860, 1180, 1436, 2204, 2460, 2780, 2908};
    BOOST_TEST(CheckMoveList(expectedMoveList3));

    board.SetBoard("n6n/8/8/1n4n1/8/6B1/5QK1/n6n b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList4{17024, 17472, 839,  1415, 1057, 1185, 1761, 2785, 3105, 3233,
                                                  1382,  1510,  1830, 2854, 3430, 3558, 2680, 3256, 3007, 3455};
    BOOST_TEST(CheckMoveList(expectedMoveList4));
}

BOOST_AUTO_TEST_CASE(Test_bishopmovement)
{
    using namespace vixen;
    board.SetBoard("8/8/8/8/3B4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList{92,   476,  668,  924,  1244, 1372, 2268,
                                                 2396, 2716, 2972, 3164, 3548, 3612};
    BOOST_TEST(CheckMoveList(expectedMoveList));

    board.SetBoard("8/Bp4pB/1P4P1/8/8/1p4p1/BP4PB/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList2{17480, 17807, 72, 399, 3696, 4023};
    BOOST_TEST(CheckMoveList(expectedMoveList2));

    board.SetBoard("8/8/8/8/3b4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList3{92,   476,  668,  924,  1244, 1372, 2268,
                                                  2396, 2716, 2972, 3164, 3548, 3612};
    BOOST_TEST(CheckMoveList(expectedMoveList3));

    board.SetBoard("8/bp4pb/1P4P1/8/8/1p4p1/bP4Pb/8 b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList4{19056, 19383, 72, 399, 3696, 4023};
    BOOST_TEST(CheckMoveList(expectedMoveList4));
}

BOOST_AUTO_TEST_CASE(Test_rookmovement)
{
    using namespace vixen;
    board.SetBoard("8/8/8/8/3R4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList{284,  796,  1308, 1564, 1628, 1692, 1756,
                                                 1884, 1948, 2012, 2332, 2844, 3356, 3868};
    BOOST_TEST(CheckMoveList(expectedMoveList));

    board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList2{64, 128, 192, 256, 320};
    BOOST_TEST(CheckMoveList(expectedMoveList2));

    board.SetBoard("8/8/8/8/3r4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList3{284,  796,  1308, 1564, 1628, 1692, 1756,
                                                  1884, 1948, 2012, 2332, 2844, 3356, 3868};
    BOOST_TEST(CheckMoveList(expectedMoveList3));

    board.SetBoard("r1b5/1p1p4/1P1P4/BP6/1P6/p1p2p1p/P1P2P1P/1B5R b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList4{18943, 3071, 3583, 4031};
    BOOST_TEST(CheckMoveList(expectedMoveList4));
}

BOOST_AUTO_TEST_CASE(Test_queenmovement)
{
    using namespace vixen;
    board.SetBoard("8/8/8/8/3Q4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList{92,   476,  668,  924,  1244, 1372, 2268, 2396, 2716,
                                                 2972, 3164, 3548, 3612, 284,  796,  1308, 1564, 1628,
                                                 1692, 1756, 1884, 1948, 2012, 2332, 2844, 3356, 3868};
    BOOST_TEST(CheckMoveList(expectedMoveList));

    board.SetBoard("8/8/8/8/3q4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList2{92,   476,  668,  924,  1244, 1372, 2268, 2396, 2716,
                                                  2972, 3164, 3548, 3612, 284,  796,  1308, 1564, 1628,
                                                  1692, 1756, 1884, 1948, 2012, 2332, 2844, 3356, 3868};
    BOOST_TEST(CheckMoveList(expectedMoveList2));
}

BOOST_AUTO_TEST_CASE(Test_kingmovement)
{
    using namespace vixen;
    board.SetBoard("8/8/8/8/3K4/8/8/8 w - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList{1244, 1308, 1372, 1756, 1884, 2268, 2332, 2396};
    BOOST_TEST(CheckMoveList(expectedMoveList));

    board.SetBoard("8/8/8/8/3k4/8/8/8 b - - 0 1");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList2{1244, 1308, 1372, 1756, 1884, 2268, 2332, 2396};
    BOOST_TEST(CheckMoveList(expectedMoveList2));

    board.SetBoard("r3k2r/8/8/8/8/p6p/P6P/R3K2R w KQkq - 0 52");
    board.PrintBoard();
    std::vector<Representation> expectedMoveList3{131, 259, 643, 707, 771, 64, 128, 263, 327, 391, 8259, 12611};
    BOOST_TEST(CheckMoveList(expectedMoveList3));
}

BOOST_AUTO_TEST_CASE(Test_huge_amunt_of_moves)
{
    using namespace vixen;
    board.SetBoard("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1");
    board.PrintBoard();
    BOOST_TEST(board.GetMoveList<MoveTypes::ALL_MOVE>().size() == 218);

    board.SetBoard("3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1");
    board.PrintBoard();
    BOOST_TEST(board.GetMoveList<MoveTypes::ALL_MOVE>().size() == 218);

    board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    board.PrintBoard();
    BOOST_TEST(board.GetMoveList<MoveTypes::ALL_MOVE>().size() == 48);
}

BOOST_AUTO_TEST_CASE(Test_making_moves)
{
    using namespace vixen;
    board.SetBoard("7k/8/8/8/3P4/8/8/7K w - - 0 1");
    static_cast<void>(board.MakeMove(Move{2332}));
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_PAWN_INDEX) == 68719476736ULL);

    board.SetBoard("1n1qb1r1/2P2P2/8/k7/8/8/8/4K3 w - - 0 1");
    BOOST_TEST(board.MakeMove(Move{65266}));
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_PAWN_INDEX) == 9007199254740992ULL);
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_QUEEN_INDEX) == 576460752303423488ULL);
    BOOST_TEST(board.GetBitBoard(Constants::BLACK_BISHOP_INDEX) == Constants::EMPTY_BOARD);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_EMPTY_INDEX) == 11952552861285482487ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_WHITE_INDEX) == 585467951558164488ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_BLACK_INDEX) == 5908723260865904640ULL);

    board.TakeBack();
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_PAWN_INDEX) == 10133099161583616ULL);
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_QUEEN_INDEX) == Constants::EMPTY_BOARD);
    BOOST_TEST(board.GetBitBoard(Constants::BLACK_BISHOP_INDEX) == 576460752303423488ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_EMPTY_INDEX) == 11951426961378639863ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_WHITE_INDEX) == 10133099161583624ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_BLACK_INDEX) == 6485184013169328128ULL);

    board.SetBoard("r3k2r/8/8/8/8/p6p/P6P/R3K2R w KQkq - 0 1");
    BOOST_TEST(board.MakeMove(Move{8259}));
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_KING_INDEX) == 2ULL);
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_ROOK_INDEX) == 132ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_EMPTY_INDEX) == 8574853690504937081ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_WHITE_INDEX) == 33158ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_BLACK_INDEX) == 9871890383204581376ULL);
    BOOST_TEST(board.GetCastlingRights() == 0b0011);

    board.SetBoard("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 b - - 0 1");
    BOOST_TEST(!board.MakeMove(Move{16775}));
    board.PrintBoard();

    // CASTLING
    board.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    BOOST_TEST(board.MakeMove(Move{8259}));
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_KING_INDEX) == 2ULL);
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_ROOK_INDEX) == 132ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_EMPTY_INDEX) == 8521146841253412985ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_WHITE_INDEX) == 103215857542ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_BLACK_INDEX) == 9925597129240281088ULL);
    BOOST_TEST(board.GetCastlingRights() == 0b0011);

    board.TakeBack();
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_KING_INDEX) == 8ULL);
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_ROOK_INDEX) == 129ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_EMPTY_INDEX) == 8521146841253412982ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_WHITE_INDEX) == 103215857545ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_BLACK_INDEX) == 9925597129240281088ULL);
    BOOST_TEST(board.GetCastlingRights() == 0b1111);

    // ENPASSANT
    static_cast<void>(board.MakeMove(Move{6095}));
    static_cast<void>(board.MakeMove(Move{21982}));
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_PAWN_INDEX) == 68853720832ULL);
    BOOST_TEST(board.GetBitBoard(Constants::BLACK_PAWN_INDEX) == 50676490932649984ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_EMPTY_INDEX) == 8521146842318798966ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_WHITE_INDEX) == 103215824777ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_BLACK_INDEX) == 9925597128174927872ULL);

    board.TakeBack();
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_PAWN_INDEX) == 71001204480ULL);
    BOOST_TEST(board.GetBitBoard(Constants::BLACK_PAWN_INDEX) == 50676491998003200ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_EMPTY_INDEX) == 8521146839105962102ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_WHITE_INDEX) == 105363308425ULL);
    BOOST_TEST(board.GetBitBoard(Constants::ALL_BLACK_INDEX) == 9925597129240281088ULL);

    board.TakeBack();
    static_cast<void>(board.MakeMove(Move{17417}));
    static_cast<void>(board.MakeMove(Move{3771}));
    board.PrintBoard();
    BOOST_TEST(Test::PerftTest(2, board) == 1790);
    board.TakeBack();
    board.TakeBack();
    board.PrintBoard();
}

BOOST_AUTO_TEST_CASE(Test_userinterface)
{
    using namespace vixen;
    board.SetBoard("7k/8/8/8/3P4/8/8/7K w - - 0 1");
    UserInterface::PrintMoveList(board);
    UserInterface::MakeMove("d4d5", board);
    UserInterface::TakeBackMove(board);
    board.PrintBoard();
    BOOST_TEST(board.GetBitBoard(Constants::WHITE_PAWN_INDEX) == 268435456ULL);
}

BOOST_AUTO_TEST_CASE(Test_material_balance)
{
    using namespace vixen;
    board.SetBoard(Constants::START_POSITION);
    UserInterface::MakeMove("e2e4", board);
    UserInterface::MakeMove("d7d5", board);
    UserInterface::MakeMove("e4d5", board);
    BOOST_TEST(board.GetMaterialBalance() == 100);
    UserInterface::MakeMove("d8d5", board);
    BOOST_TEST(board.GetMaterialBalance() == 0);
    UserInterface::MakeMove("b1c3", board);
    UserInterface::MakeMove("g8f6", board);
    UserInterface::MakeMove("c3d5", board);
    BOOST_TEST(board.GetMaterialBalance() == 900);
    UserInterface::TakeBackMove(board);
    BOOST_TEST(board.GetMaterialBalance() == 0);

    board.SetBoard("3q2k1/4P3/8/5K2/8/8/8/8 w - - 0 1");
    board.PrintBoard();
    BOOST_TEST(board.GetMaterialBalance() == -800);
    UserInterface::MakeMove("e7d8r", board);
    BOOST_TEST(board.GetMaterialBalance() == 500);
    UserInterface::TakeBackMove(board);
    BOOST_TEST(board.GetMaterialBalance() == -800);
    UserInterface::MakeMove("e7d8q", board);
    BOOST_TEST(board.GetMaterialBalance() == 900);

    board.SetBoard(Constants::TESTPOS1);
    const int materialBefore = board.GetMaterialBalance();
    Test::PerftTest(3, board);
    BOOST_TEST(materialBefore == board.GetMaterialBalance());
}

BOOST_AUTO_TEST_CASE(Test_Get_BestMove)
{
    using namespace vixen;
    const Move nullMove{};
    const Move move{3U, 4U, static_cast<std::uint8_t>(vixen::MoveTypes::CAPTURE)};
    BOOST_TEST(move == GetBestMove(nullMove, move));
    BOOST_TEST(move == GetBestMove(move, move));
    BOOST_TEST(move == GetBestMove(move, nullMove));
}

BOOST_AUTO_TEST_SUITE_END()
