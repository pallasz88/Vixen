#define BOOST_TEST_MODULE UciTests
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <sstream>

#include "engine.hpp"
#include "fixed_list.hpp"
#include "move.hpp"
#include "uci.hpp"

namespace
{
constexpr auto uciInput = "uci\nisready\nucinewgame\nposition startpos\ngo depth 1\nstop\nquit\n";
}

BOOST_AUTO_TEST_CASE(uci_commands_complete_lifecycle)
{
    std::cin.clear();
    std::istringstream input{uciInput};
    std::ostringstream output;
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    {
        vixen::Uci uci;
        uci.loop();
    }
    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    BOOST_CHECK(output.str().find("uciok") != std::string::npos &&
                output.str().find("readyok") != std::string::npos &&
                output.str().find("bestmove") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(uci_logging_and_benchmark_cover_search_paths)
{
    std::cin.clear();
    std::ostringstream output;
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    {
        vixen::Uci uci;
        FixedList<vixen::Move> pv{};
        pv.emplace_back(vixen::Move(10U, 18U, 0U));
        vixen::SearchInfo info{};
        info.maxDepth = 4;
        info.nodesCount = 57U;
        vixen::Uci::LogUci(info, 128, 2, pv);
        uci.benchmark();
    }
    std::cout.rdbuf(oldOutput);

    const auto text = output.str();
    BOOST_CHECK(text.find("info score cp 128") != std::string::npos);
    BOOST_CHECK(text.find("bestmove") != std::string::npos || text.find("info score") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(uci_logs_mate_scores)
{
    std::ostringstream output;
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::SearchInfo info{};
    FixedList<vixen::Move> pv{};
    vixen::Uci::LogUci(info, vixen::Search::MATE, 1, pv);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().find("score mate") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(uci_logs_negative_mate_scores)
{
    std::ostringstream output;
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::SearchInfo info{};
    FixedList<vixen::Move> pv{};
    vixen::Uci::LogUci(info, -vixen::Search::MATE, 1, pv);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().find("score mate") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(uci_parses_white_clock_options)
{
    std::cin.clear();
    std::istringstream input{
        "position startpos\n"
        "go wtime 1000 winc 20 movestogo 10 movetime 50 depth 1\n"
        "stop\nquit\n"};
    std::ostringstream output;
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::Uci uci;
    uci.loop();
    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().find("bestmove") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(uci_parses_black_clock_options_and_fen_moves)
{
    std::cin.clear();
    std::istringstream input{
        "position fen 4k3/8/8/8/8/8/8/4K3 b - - 0 1 moves e8e7\n"
        "go btime 1000 binc 20 depth 1\n"
        "stop\nquit\n"};
    std::ostringstream output;
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::Uci uci;
    uci.loop();
    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().find("bestmove") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(uci_parses_black_clock_without_moving)
{
    std::cin.clear();
    std::istringstream input{
        "position fen 4k3/8/8/8/8/8/8/4K3 b - - 0 1\n"
        "go btime 1000 binc 20 depth 1\n"
        "stop\nquit\n"};
    std::ostringstream output;
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::Uci uci;
    uci.loop();
    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().find("bestmove") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(uci_ignores_unknown_position_format)
{
    std::cin.clear();
    std::istringstream input{"position nonsense\nquit\n"};
    std::ostringstream output;
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::Uci uci;
    uci.loop();
    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().empty());
}

BOOST_AUTO_TEST_CASE(uci_processes_stop_without_active_search)
{
    std::cin.clear();
    std::istringstream input{"stop\nquit\n"};
    std::ostringstream output;
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    auto *oldOutput = std::cout.rdbuf(output.rdbuf());
    vixen::Uci uci;
    uci.loop();
    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);
    BOOST_CHECK(output.str().empty());
}
