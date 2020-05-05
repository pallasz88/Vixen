#pragma once

#include <boost/test/included/unit_test.hpp>
#include "board.h"
#include "move_generator.h"

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

        bool CheckMoveList(std::vector<Move> &expectedMoveList)
        {
            const auto generator = board.CreateGenerator<ALL_MOVE>();
            const auto listSize  = generator.GetListSize();
            const auto moveList  = generator.GetMoveList();

            if (expectedMoveList.size() != listSize)
                return false;

            for (unsigned i = 0; i < listSize; ++i)
            {
                if (expectedMoveList[i] != moveList[i])
                    return false;
            }

            return true;
        }

        Board board;
    };
}
