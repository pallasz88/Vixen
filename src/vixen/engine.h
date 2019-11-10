#pragma once

#include "defs.h"

namespace Vixen
{
    class Board;
}

namespace Vixen::Search
{
    extern int Evaluate(const Board &board);

    extern int Quiescence(int alpha, int beta, Board& board);

    VIXEN_API extern int NegaMax(int depth, int alpha, int beta, Board& board);

    extern std::pair<int, Move> Root(int depth, Board &board);
}