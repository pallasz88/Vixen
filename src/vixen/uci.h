#pragma once

#include "defs.h"

namespace Vixen
{
    class Board;

    struct SearchInfo;
}

namespace Vixen::Uci
{
    VIXEN_API void loop(Board &board);

    extern void LogUci(const SearchInfo &info, const std::pair<int, Move> &result, int depth,
                       const std::vector<Move> &bestLine);
}
