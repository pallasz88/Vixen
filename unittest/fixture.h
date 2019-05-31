#pragma once

#include <boost/test/included/unit_test.hpp>
#include "userinterface.h"
#include "board.h"

namespace Vixen
{
    struct Fixture
    {
        Fixture() : board(Board())
        {
            BOOST_TEST_MESSAGE("setup fixture");
        }

        ~Fixture()
        {
            BOOST_TEST_MESSAGE("teardown fixture");
        }

        Board board;
    };
}
