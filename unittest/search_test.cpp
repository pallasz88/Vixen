#define BOOST_TEST_MODULE SearchTests
#include <boost/test/included/unit_test.hpp>

#include "board.hpp"
#include "engine.hpp"

BOOST_AUTO_TEST_CASE(iterative_search_populates_node_count)
{
    vixen::Board board;
    vixen::SearchInfo info;
    info.maxDepth = 1;

    vixen::Search::IterativeDeepening(board, info);
    vixen::Search::ClearTables();

    BOOST_CHECK(info.nodesCount > 0);
}
