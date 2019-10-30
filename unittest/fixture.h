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
            MoveGenerator generator = board.CreateGenerator<ALL_MOVE>();
            int listSize = generator.GetListSize();
            std::array<Move, MAX_MOVELIST_SIZE> moveList = generator.GetMoveList();

            if (expectedMoveList.size() != listSize)
                return false;

            for (int i = 0; i < listSize; ++i)
            {
                if (expectedMoveList[i] != moveList[i])
                    return  false;
            }

            return true;
        }

        Board board;
    };
}
