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
        "position rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n"
        "perft 1\n"
        "move e2e4\n"
        "undo\n"
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
