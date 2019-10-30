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

    VIXEN_API extern int AlphaBeta(int depth, int alpha, int beta, Board& board);
}