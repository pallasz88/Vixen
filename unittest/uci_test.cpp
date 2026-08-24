#define BOOST_TEST_MODULE UciTests
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <sstream>

#include "uci.hpp"

namespace
{
constexpr auto uciInput = "uci\nisready\nucinewgame\nposition startpos\ngo depth 1\nstop\nquit\n";
}

BOOST_AUTO_TEST_CASE(uci_commands_complete_lifecycle)
{
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
