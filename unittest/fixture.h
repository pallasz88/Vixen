#pragma once

#include <boost/test/included/unit_test.hpp>
#include <vector>

#include "board.hpp"

namespace vixen
{
struct Fixture
{
    Fixture()
    {
        BOOST_TEST_MESSAGE("setup fixture");
    }

    ~Fixture()
    {
        BOOST_TEST_MESSAGE("teardown fixture");
    }

    Fixture(const Fixture &) = delete;

    Fixture &operator=(const Fixture &) = delete;

    Fixture(Fixture &&) = delete;

    Fixture &operator=(Fixture &&) = delete;

    bool CheckMoveList(const std::vector<Representation> &expectedMoveList)
    {
        const auto moveList = board.GetMoveList<MoveTypes::ALL_MOVE>();

        if (expectedMoveList.size() != moveList.size())
            return false;

        for (unsigned i = 0; i < moveList.size(); ++i)
        {
            if (moveList[i] != expectedMoveList[i])
                return false;
        }

        return true;
    }

    Board board{};
};
} // namespace vixen
