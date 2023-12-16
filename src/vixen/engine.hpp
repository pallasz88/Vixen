#ifndef VIXEN_ENGINE_HPP_INCLUDED
#define VIXEN_ENGINE_HPP_INCLUDED

#include <atomic>
#include <chrono>
#include <functional>

#include "defs.hpp"
#include "fixed_list.hpp"
#include "principal_variation.hpp"

namespace vixen
{
class Board;

struct SearchInfo
{
    std::chrono::high_resolution_clock::time_point startTime{};
    std::chrono::high_resolution_clock::time_point endTime{};
    unsigned long long nodesCount{0};
    size_t timeIndex{0};
    int maxDepth{64};
    int currentDepth{0};
    int movesToGo{30};
    int moveTime{0};
    std::array<int, 2> time{-1, -1};
    std::array<int, 2> increment{};
    std::atomic_bool stopped{false};
    bool isTimeSet{};
};

struct Utility
{
    template <class T> static constexpr T MirrorTable(const T &table) noexcept
    {
        T reversed{};
        std::reverse_copy(begin(table), end(table), begin(reversed));
        std::transform(cbegin(reversed), cend(reversed), begin(reversed), std::negate<typename T::value_type>());
        return reversed;
    }

    static constexpr auto InitMvvLvaTable()
    {
        const std::array<unsigned, 12> victimScore{100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};
        std::array<std::array<unsigned, 12>, 12> mvvlvaTable{};

        for (unsigned attackerIndex = 0; attackerIndex < Constants::pieceKeys.size(); ++attackerIndex)
        {
            for (unsigned victimIndex = 0; victimIndex < Constants::pieceKeys.size(); ++victimIndex)
            {
                mvvlvaTable[attackerIndex][victimIndex] =
                    victimScore[victimIndex] + 6U - victimScore[attackerIndex] / 100;
            }
        }

        return mvvlvaTable;
    }
};

class VIXEN_API Search
{
  public:
    static void IterativeDeepening(Board &board, SearchInfo &info);

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

    static constexpr std::array mvvlvaTable = Utility::InitMvvLvaTable();

    static constexpr std::array pawnTable = Utility::MirrorTable(PawnTable);

    static constexpr std::array knightTable = Utility::MirrorTable(KnightTable);

    static constexpr std::array bishopTable = Utility::MirrorTable(BishopTable);

    static constexpr std::array rookTable = Utility::MirrorTable(RookTable);

    static constexpr std::array arrayLookUp = {
        pawnTable, knightTable, bishopTable, rookTable, PawnTable, KnightTable, BishopTable, RookTable,
    };

    static int Evaluate(const Board &board);

    static int Quiescence(int alpha, int beta, Board &board, SearchInfo &info);

    static int NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info);

    static std::pair<int, Move> Root(int depth, Board &board, SearchInfo &info);

    static FixedList<Move> GetPV(int, Board &);

    static void OrderCapture(const Board &board, Move &move);

    static void OrderNonPVMoves(int depth, const Board &board, Move &move);

    static bool IsPVMove(const PVEntry &pvEntry, Move &move);
};

template std::array<int, 64ul> vixen::Utility::MirrorTable<std::array<int, 64ul>>(
    std::array<int, 64ul> const &) noexcept;
} // namespace vixen

#endif // VIXEN_ENGINE_HPP_INCLUDED