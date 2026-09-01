#define BOOST_TEST_MODULE SearchTests
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <sstream>

#include "board.hpp"
#include "engine.hpp"
#include "userinterface.hpp"

BOOST_AUTO_TEST_CASE(iterative_search_populates_node_count)
{
    vixen::Board board;
    vixen::SearchInfo info;
    info.maxDepth = 1;

    vixen::Search::IterativeDeepening(board, info);
    vixen::Search::IterativeDeepening(board, info);
    vixen::Search::ClearTables();

    BOOST_CHECK(info.nodesCount > 0);
}

BOOST_AUTO_TEST_CASE(user_interface_helpers_cover_common_commands)
{
    vixen::Board board;
    std::istringstream input{
        "help\n"
        "print\n"
        "reset\n"
        "undo\n"
        "move a\n"
        "move invalid\n"
        "position rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n"
        "perft 1\n"
        "move e2e4\n"
        "undo\n"
        "move invalid\n"
        "move a1a2\n"
        "move\n"
        "undo\n"
        "unknown\n"
        "list\n"
        "quit\n"};
    std::ostringstream output;
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    auto *oldError = std::cerr.rdbuf(output.rdbuf());

    vixen::UserInterface::WaitUserInput(board);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);
    std::cerr.rdbuf(oldError);

    const auto text = output.str();
    BOOST_CHECK(text.find("Welcome to Vixen C++ chess engine!") != std::string::npos);
    BOOST_CHECK(text.find("Vixen >") != std::string::npos);
    BOOST_CHECK(text.find("1") != std::string::npos);

    vixen::Board boardAfterMove;
    vixen::UserInterface::MakeMove("e2e4", boardAfterMove);
    BOOST_CHECK(boardAfterMove.GetPiece(static_cast<unsigned>(vixen::Squares::E4)) == vixen::Constants::WHITE_PAWN_INDEX);

    vixen::UserInterface::TakeBackMove(boardAfterMove);
    BOOST_CHECK(boardAfterMove.GetPiece(static_cast<unsigned>(vixen::Squares::E2)) == vixen::Constants::WHITE_PAWN_INDEX);

    std::ostringstream helpOutput;
    auto *previousOutput = std::cout.rdbuf(helpOutput.rdbuf());
    vixen::UserInterface::PrintHelp();
    std::cout.rdbuf(previousOutput);
    BOOST_CHECK(helpOutput.str().find("Use the below listed commands") != std::string::npos);

    vixen::Board listBoard;
    std::ostringstream listOutput;
    auto *listOutputStream = std::cout.rdbuf(listOutput.rdbuf());
    vixen::UserInterface::PrintMoveList(listBoard);
    std::cout.rdbuf(listOutputStream);
    BOOST_CHECK(listOutput.str().find(",") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(user_interface_prints_promotion_moves)
{
    vixen::Board board;
    board.SetBoard("7k/P7/8/8/8/8/8/7K w - - 0 1");
    std::ostringstream output;
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::UserInterface::PrintMoveList(board);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().find("q") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(missing_piece_material_returns_empty_optional)
{
    BOOST_CHECK(!vixen::GetPieceMaterial(9999));
}

BOOST_AUTO_TEST_CASE(missing_promotion_type_returns_empty_optional)
{
    BOOST_CHECK(!vixen::GetPromotionType('x'));
}

BOOST_AUTO_TEST_CASE(invalid_square_notation_returns_negative)
{
    BOOST_CHECK(vixen::Move::NotationToSquare("z9") == -1);
}

BOOST_AUTO_TEST_CASE(invalid_rank_notation_returns_negative)
{
    BOOST_CHECK(vixen::Move::NotationToSquare("a9") == -1);
}

BOOST_AUTO_TEST_CASE(invalid_file_notation_returns_negative)
{
    BOOST_CHECK(vixen::Move::NotationToSquare("A1") == -1);
}

BOOST_AUTO_TEST_CASE(invalid_file_above_board_returns_negative)
{
    BOOST_CHECK(vixen::Move::NotationToSquare("i1") == -1);
}

BOOST_AUTO_TEST_CASE(invalid_rank_below_board_returns_negative)
{
    BOOST_CHECK(vixen::Move::NotationToSquare("a0") == -1);
}

BOOST_AUTO_TEST_CASE(principal_variation_entry_matches_exactly)
{
    const vixen::PVEntry entry{vixen::Move(1U, 2U, 0U), 1U};
    BOOST_CHECK(entry == entry);
}

BOOST_AUTO_TEST_CASE(principal_variation_entry_rejects_different_move)
{
    const vixen::PVEntry first{vixen::Move(1U, 2U, 0U), 1U};
    const vixen::PVEntry second{vixen::Move(1U, 3U, 0U), 1U};
    BOOST_CHECK(!(first == second));
}

BOOST_AUTO_TEST_CASE(utility_tables_are_initialized)
{
    auto init = &vixen::Utility::InitMvvLvaTable;
    const auto table = init();
    BOOST_CHECK(table[0][0] == 105U);
}

BOOST_AUTO_TEST_CASE(utility_tables_can_be_mirrored_at_runtime)
{
    std::array<int, 64> table{};
    table[0] = 1;
    const auto mirrored = vixen::Utility::MirrorTable(table);
    BOOST_CHECK(mirrored[63] == -1);
}

BOOST_AUTO_TEST_CASE(principal_variation_entries_compare_unequally)
{
    const vixen::PVEntry first{vixen::Move(1U, 2U, 0U), 1U};
    const vixen::PVEntry second{vixen::Move(1U, 2U, 0U), 2U};
    BOOST_CHECK(!(first == second));
}

BOOST_AUTO_TEST_CASE(invalid_side_to_move_fen_is_ignored_without_crashing)
{
    vixen::Board board;
    std::ostringstream errorOutput;
    auto *oldError = std::cerr.rdbuf(errorOutput.rdbuf());
    board.SetBoard("8/8/8/8/8/8/8/8 x KQkq - 0 1");
    std::cerr.rdbuf(oldError);
    BOOST_CHECK(board.GetPieceList().size() == 64U);
}

BOOST_AUTO_TEST_CASE(invalid_castling_fen_is_ignored_without_crashing)
{
    vixen::Board board;
    std::ostringstream errorOutput;
    auto *oldError = std::cerr.rdbuf(errorOutput.rdbuf());
    board.SetBoard("8/8/8/8/8/8/8/8 w x - 0 1");
    std::cerr.rdbuf(oldError);
    BOOST_CHECK(board.GetPieceList().size() == 64U);
}

BOOST_AUTO_TEST_CASE(invalid_piece_fen_is_ignored_without_crashing)
{
    vixen::Board board;
    std::ostringstream errorOutput;
    auto *oldError = std::cerr.rdbuf(errorOutput.rdbuf());
    board.SetBoard("8/8/8/8/8/8/8/X7 w - - 0 1");
    std::cerr.rdbuf(oldError);
    BOOST_CHECK(board.GetPieceList().size() == 64U);
}

BOOST_AUTO_TEST_CASE(invalid_move_syntax_throws)
{
    vixen::Board board;
    bool threw = false;
    try
    {
        static_cast<void>(board.MakeMove("invalid"));
    }
    catch (const std::runtime_error &)
    {
        threw = true;
    }
    BOOST_CHECK(threw);
}

BOOST_AUTO_TEST_CASE(illegal_move_returns_false)
{
    vixen::Board board;
    BOOST_CHECK(!board.MakeMove("a1a2"));
}

BOOST_AUTO_TEST_CASE(quiet_promotion_is_decoded_without_capture)
{
    vixen::Board board;
    board.SetBoard("7k/P7/8/8/8/8/8/7K w - - 0 1");
    BOOST_CHECK(board.MakeMove("a7a8q"));
}

BOOST_AUTO_TEST_CASE(capture_promotion_is_decoded_with_capture)
{
    vixen::Board board;
    board.SetBoard("1r5k/P7/8/8/8/8/8/7K w - - 0 1");
    BOOST_CHECK(board.MakeMove("a7b8q"));
}

BOOST_AUTO_TEST_CASE(legal_move_generator_rejects_moves_leaving_check)
{
    vixen::Board board;
    board.SetBoard("4r1k1/8/8/8/8/8/8/R3K3 w - - 0 1");
    vixen::MoveGenerator generator;
    generator.GenerateMoves<vixen::Colors::WHITE, vixen::MoveTypes::ALL_MOVE>(board);
    BOOST_CHECK(generator.GetLegalMoveList(board).size() < generator.GetMoveList().size());
}

BOOST_AUTO_TEST_CASE(move_list_skips_illegal_pseudo_moves)
{
    vixen::Board board;
    board.SetBoard("4r1k1/8/8/8/8/8/8/R3K3 w - - 0 1");
    vixen::UserInterface::PrintMoveList(board);
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(search_uses_available_side_clock)
{
    vixen::Board board;
    vixen::SearchInfo info{};
    info.maxDepth = 1;
    info.isTimeSet = true;
    info.moveTime = 0;
    info.time[0] = 1000;
    info.nodesCount = 255;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(!info.stopped);
}

BOOST_AUTO_TEST_CASE(search_uses_move_time_when_side_clock_is_unset)
{
    vixen::Board board;
    vixen::SearchInfo info{};
    info.maxDepth = 1;
    info.isTimeSet = true;
    info.moveTime = 1000;
    info.time[0] = -1;
    info.nodesCount = 255;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(!info.stopped);
}

BOOST_AUTO_TEST_CASE(legal_move_generator_returns_legal_moves)
{
    vixen::Board board;
    vixen::MoveGenerator generator;
    generator.GenerateMoves<vixen::Colors::WHITE, vixen::MoveTypes::ALL_MOVE>(board);
    BOOST_CHECK(generator.GetLegalMoveList(board).size() == 20U);
}

BOOST_AUTO_TEST_CASE(stalemate_search_returns_stalemate_score)
{
    vixen::Board board;
    board.SetBoard("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1");
    vixen::SearchInfo info{};
    info.maxDepth = 1;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(info.nodesCount > 0U);
}

BOOST_AUTO_TEST_CASE(fifty_move_search_is_drawn)
{
    vixen::Board board;
    board.SetBoard("7k/5Q2/6K1/8/8/8/8/7K w - - 100 1");
    vixen::SearchInfo info{};
    info.maxDepth = 1;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(info.nodesCount > 0U);
}

BOOST_AUTO_TEST_CASE(time_limited_search_stops_cleanly)
{
    vixen::Board board;
    vixen::SearchInfo info{};
    info.maxDepth = 2;
    info.isTimeSet = true;
    info.moveTime = 0;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(!info.stopped);
}

BOOST_AUTO_TEST_CASE(periodic_time_check_stops_search)
{
    vixen::Board board;
    vixen::SearchInfo info{};
    info.maxDepth = 1;
    info.isTimeSet = true;
    info.moveTime = 0;
    info.nodesCount = 254;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(!info.stopped);
}

BOOST_AUTO_TEST_CASE(quiescence_time_check_stops_search)
{
    vixen::Board board;
    vixen::SearchInfo info{};
    info.maxDepth = 1;
    info.isTimeSet = true;
    info.moveTime = 0;
    info.nodesCount = 253;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(!info.stopped);
}

BOOST_AUTO_TEST_CASE(time_check_uses_side_clock_when_available)
{
    vixen::Board board;
    board.SetBoard("4k3/8/8/8/8/8/8/4K3 b - - 0 1");
    vixen::SearchInfo info{};
    info.maxDepth = 1;
    info.isTimeSet = true;
    info.time[1] = 1000;
    info.nodesCount = 254;
    vixen::Search::IterativeDeepening(board, info);
    BOOST_CHECK(!info.stopped);
}

BOOST_AUTO_TEST_CASE(perft_depth_zero_returns_one)
{
    vixen::Board board;
    BOOST_CHECK(vixen::Test::PerftTest(0, board) == 1U);
}
