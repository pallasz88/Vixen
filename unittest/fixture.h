#pragma once

#include <boost/test/included/unit_test.hpp>
#include "userinterface.h"
#include "board.h"

namespace Vixen
{
    struct Fixture
    {
        Fixture() : board(Board()), user(UserInterface())
        {
            BOOST_TEST_MESSAGE("setup fixture");
        }

        ~Fixture()
        {
            BOOST_TEST_MESSAGE("teardown fixture");
        }

        Board board;

        UserInterface user;
    };
}
