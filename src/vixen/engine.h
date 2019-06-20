#pragma once

#include "board.h"
#include "move_generator.h"

namespace Vixen::Search
{
    extern int Evaluate(const Board &board);

    extern int Quiescence(int alpha, int beta, Board& board);

    VIXEN_API extern int AlphaBeta(int depth, int alpha, int beta, Board& board);
}