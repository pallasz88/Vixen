#pragma once

#include "defs.h"

#include <chrono>

namespace Vixen
{
    class Board;

    struct SearchInfo
    {
        std::chrono::high_resolution_clock::time_point startTime{};
        std::chrono::high_resolution_clock::time_point endTime{};
        long nodesCount{0};
        int maxDepth{7};
        int currentDepth{0};
        int movesToGo{30};
        int moveTime{0};
        std::array<int, 2> time{};
        std::array<int, 2> increment{};
        bool stopped{false};
    };
}

namespace Vixen::Search
{
    static constexpr std::array<int, Constants::SQUARE_NUMBER> pawnTable =
    {
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
        10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
        5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
        0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
        5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
        10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
        20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
    };

    static constexpr std::array<int, Constants::SQUARE_NUMBER> knightTable =
    {
        0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
        0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
        0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
        0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
        5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
        5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
    };

    static constexpr std::array<int, Constants::SQUARE_NUMBER> bishopTable =
    {
        0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
        0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
        0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
        0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
        0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
        0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
        0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
    };

    static constexpr std::array<int, Constants::SQUARE_NUMBER> rookTable =
    {
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
    };

    template <class T>
    static auto MirrorTable(const T& list) noexcept
    {
        T table{};
        std::reverse_copy(begin(list), end(list), begin(table));
        return table;
    }

    static const std::array<int, Constants::SQUARE_NUMBER> PawnTable = MirrorTable(pawnTable);

    static const std::array<int, Constants::SQUARE_NUMBER> KnightTable = MirrorTable(knightTable);

    static const std::array<int, Constants::SQUARE_NUMBER> BishopTable = MirrorTable(bishopTable);

    static const std::array<int, Constants::SQUARE_NUMBER> RookTable = MirrorTable(rookTable);

    extern int Evaluate(const Board &board);

    extern int Quiescence(int alpha, int beta, Board &board, SearchInfo &info);

    VIXEN_API extern int NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info);

    extern std::pair<int, Move> Root(int depth, Board &board, SearchInfo &info);

    extern std::pair<int, Move> IterativeDeepening(Board &board, SearchInfo &info);
}