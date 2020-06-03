#pragma once

#include "defs.h"
#include "principal_variation.h"


#include <chrono>

namespace Vixen
{
    class Board;

    struct SearchInfo
    {
        std::chrono::high_resolution_clock::time_point startTime{};
        std::chrono::high_resolution_clock::time_point endTime{};
        long                                           nodesCount{0};
        int                                            maxDepth{7};
        int                                            currentDepth{0};
        int                                            movesToGo{30};
        int                                            moveTime{0};
        std::array<int, 2>                             time{};
        std::array<int, 2>                             increment{};
        bool                                           stopped{false};
    };

    class Search
    {
    public:

        static Move IterativeDeepening(Board &board, SearchInfo &info);

        static constexpr int MATE = 2999999;

        static constexpr int STALE_MATE = 0;

    private:

        static PrincipalVariation pv;

        static constexpr std::array<int, Constants::SQUARE_NUMBER> pawnTable =
                                                                           {
                                                                                   0, 0, 0, 0, 0, 0, 0, 0,
                                                                                   10, 10, 0, -10, -10, 0, 10, 10,
                                                                                   5, 0, 0, 5, 5, 0, 0, 5,
                                                                                   0, 0, 10, 20, 20, 10, 0, 0,
                                                                                   5, 5, 5, 10, 10, 5, 5, 5,
                                                                                   10, 10, 10, 20, 20, 10, 10, 10,
                                                                                   20, 20, 20, 30, 30, 20, 20, 20,
                                                                                   0, 0, 0, 0, 0, 0, 0, 0
                                                                           };

        static constexpr std::array<int, Constants::SQUARE_NUMBER> knightTable =
                                                                           {
                                                                                   0, -10, 0, 0, 0, 0, -10, 0,
                                                                                   0, 0, 0, 5, 5, 0, 0, 0,
                                                                                   0, 0, 10, 10, 10, 10, 0, 0,
                                                                                   0, 0, 10, 20, 20, 10, 5, 0,
                                                                                   5, 10, 15, 20, 20, 15, 10, 5,
                                                                                   5, 10, 10, 20, 20, 10, 10, 5,
                                                                                   0, 0, 5, 10, 10, 5, 0, 0,
                                                                                   0, 0, 0, 0, 0, 0, 0, 0
                                                                           };

        static constexpr std::array<int, Constants::SQUARE_NUMBER> bishopTable =
                                                                           {
                                                                                   0, 0, -10, 0, 0, -10, 0, 0,
                                                                                   0, 0, 0, 10, 10, 0, 0, 0,
                                                                                   0, 0, 10, 15, 15, 10, 0, 0,
                                                                                   0, 10, 15, 20, 20, 15, 10, 0,
                                                                                   0, 10, 15, 20, 20, 15, 10, 0,
                                                                                   0, 0, 10, 15, 15, 10, 0, 0,
                                                                                   0, 0, 0, 10, 10, 0, 0, 0,
                                                                                   0, 0, 0, 0, 0, 0, 0, 0
                                                                           };

        static constexpr std::array<int, Constants::SQUARE_NUMBER> rookTable =
                                                                           {
                                                                                   0, 0, 5, 10, 10, 5, 0, 0,
                                                                                   0, 0, 5, 10, 10, 5, 0, 0,
                                                                                   0, 0, 5, 10, 10, 5, 0, 0,
                                                                                   0, 0, 5, 10, 10, 5, 0, 0,
                                                                                   0, 0, 5, 10, 10, 5, 0, 0,
                                                                                   0, 0, 5, 10, 10, 5, 0, 0,
                                                                                   25, 25, 25, 25, 25, 25, 25, 25,
                                                                                   0, 0, 5, 10, 10, 5, 0, 0
                                                                           };

        template<class T>
        static constexpr T MirrorTable(const T &table) noexcept
        {
            T reversed{};
            std::reverse_copy(begin(table), end(table), begin(reversed));
            return reversed;
        }

        static inline std::array PawnTable = MirrorTable(pawnTable);

        static inline std::array KnightTable = MirrorTable(knightTable);

        static inline std::array BishopTable = MirrorTable(bishopTable);

        static inline std::array RookTable = MirrorTable(rookTable);

        static int Evaluate(const Board &board);

        static int Quiescence(int alpha, int beta, Board &board, SearchInfo &info);

        static int NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info);

        static std::pair<int, Move> Root(int depth, Board &board, SearchInfo &info);

        static std::vector<Move> GetPV(int, Board &);

    };
}