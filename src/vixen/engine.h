#pragma once

#include <chrono>

#include "defs.h"
#include "principal_variation.h"

namespace Vixen
{
class Board;

struct SearchInfo
{
    std::chrono::high_resolution_clock::time_point startTime{};
    std::chrono::high_resolution_clock::time_point endTime{};
    unsigned long long nodesCount{0};
    int maxDepth{7};
    int currentDepth{0};
    int movesToGo{30};
    int moveTime{0};
    std::array<int, 2> time{};
    std::array<int, 2> increment{};
    bool stopped{false};
};

struct Utility
{
    template <class T>
#if (__cpp_lib_constexpr_algorithms)
    static constexpr T MirrorTable(const T &table) noexcept
#else
    static T MirrorTable(const T &table) noexcept
#endif
    {
        T reversed{};
        std::reverse_copy(begin(table), end(table), begin(reversed));
        return reversed;
    }
};

class VIXEN_API Search
{
  public:
    static Move IterativeDeepening(Board &board, SearchInfo &info);

    static constexpr int MATE = 2999999;

    static constexpr int STALE_MATE = 0;

  private:
    static constexpr int megaByte = 1ULL << 20ULL;

    static PrincipalVariation pv;

    static constexpr std::array PawnTable = {0,  0,  0,  0,  0,  0,  0,  0,  10, 10, 0,  -10, -10, 0,  10, 10,
                                             5,  0,  0,  5,  5,  0,  0,  5,  0,  0,  10, 20,  20,  10, 0,  0,
                                             5,  5,  5,  10, 10, 5,  5,  5,  10, 10, 10, 20,  20,  10, 10, 10,
                                             20, 20, 20, 30, 30, 20, 20, 20, 0,  0,  0,  0,   0,   0,  0,  0};

    static constexpr std::array KnightTable = {0, -10, 0,  0,  0,  0,  -10, 0, 0, 0,  0,  5,  5,  0,  0,  0,
                                               0, 0,   10, 10, 10, 10, 0,   0, 0, 0,  10, 20, 20, 10, 5,  0,
                                               5, 10,  15, 20, 20, 15, 10,  5, 5, 10, 10, 20, 20, 10, 10, 5,
                                               0, 0,   5,  10, 10, 5,  0,   0, 0, 0,  0,  0,  0,  0,  0,  0};

    static constexpr std::array BishopTable = {0, 0,  -10, 0,  0,  -10, 0,  0, 0, 0,  0,  10, 10, 0,  0,  0,
                                               0, 0,  10,  15, 15, 10,  0,  0, 0, 10, 15, 20, 20, 15, 10, 0,
                                               0, 10, 15,  20, 20, 15,  10, 0, 0, 0,  10, 15, 15, 10, 0,  0,
                                               0, 0,  0,   10, 10, 0,   0,  0, 0, 0,  0,  0,  0,  0,  0,  0};

    static constexpr std::array RookTable = {
        0, 0, 5, 10, 10, 5, 0, 0, 0, 0, 5, 10, 10, 5, 0, 0, 0,  0,  5,  10, 10, 5,  0,  0,  0, 0, 5, 10, 10, 5, 0, 0,
        0, 0, 5, 10, 10, 5, 0, 0, 0, 0, 5, 10, 10, 5, 0, 0, 25, 25, 25, 25, 25, 25, 25, 25, 0, 0, 5, 10, 10, 5, 0, 0};
#if (__cpp_lib_constexpr_algorithms)

    static constexpr std::array pawnTable = Utility::MirrorTable(PawnTable);

    static constexpr std::array knightTable = Utility::MirrorTable(KnightTable);

    static constexpr std::array bishopTable = Utility::MirrorTable(BishopTable);

    static constexpr std::array rookTable = Utility::MirrorTable(RookTable);

    static constexpr std::array arrayLookUp{pawnTable, knightTable, bishopTable, rookTable,
                                            PawnTable, KnightTable, BishopTable, RookTable};

#else

    static inline std::array pawnTable = Utility::MirrorTable(PawnTable);

    static inline std::array knightTable = Utility::MirrorTable(KnightTable);

    static inline std::array bishopTable = Utility::MirrorTable(BishopTable);

    static inline std::array rookTable = Utility::MirrorTable(RookTable);

    static inline std::array arrayLookUp{pawnTable, knightTable, bishopTable, rookTable,
                                         PawnTable, KnightTable, BishopTable, RookTable};

#endif

    static int Evaluate(const Board &board);

    static int Quiescence(int alpha, int beta, Board &board, SearchInfo &info);

    static int NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info);

    static std::pair<int, Move> Root(int depth, Board &board, SearchInfo &info);

    static std::vector<Move> GetPV(int, Board &);
};

template std::array<int, 64ul> Vixen::Utility::MirrorTable<std::array<int, 64ul>>(std::array<int, 64ul> const &);
} // namespace Vixen