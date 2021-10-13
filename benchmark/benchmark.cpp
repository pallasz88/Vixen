#include <benchmark/benchmark.h>
#define private public
#include "engine.hpp"
#undef private
#include "board.hpp"
#include <iostream>

vixen::Hash::Keys vixen::Hash::zobristKeys;


static constexpr std::array evalMap{std::make_pair('P', 4), std::make_pair('N', 5), std::make_pair('B', 6),
                                    std::make_pair('R', 7), std::make_pair('p', 0), std::make_pair('n', 1),
                                    std::make_pair('b', 2), std::make_pair('r', 3)};

constexpr int GetEvalIndexIfElse(char c) noexcept
{
    if (c == 'p') return 0;
    else if (c == 'n') return 1;
    else if (c == 'b') return 2;
    else if (c == 'r') return 3;
    else if (c == 'P') return 4;
    else if (c == 'N') return 5;
    else if (c == 'B') return 6;
    else if (c == 'R') return 7;
    else return -1;
}

constexpr auto GetEvalIndexLoop(char c) noexcept
{
    for (const auto &i : evalMap)
    {
        if (i.first == c)
            return i.second;
    }
    return -1;
}

int EvaluateIfElse(const vixen::Board &board)
{
    int score = board.GetMaterialBalance();

    unsigned i = 0;
    for (const char piece : board.GetPieceList())
    {
        if (piece == 'P' || piece == 'N' || piece == 'B' || piece == 'R' ||
            piece == 'p' || piece == 'n' || piece == 'b' || piece == 'r')
            score += vixen::Search::arrayLookUp[GetEvalIndexIfElse(piece)][i];
        ++i;
    }

    return board.IsWhiteToMove() ? score : -score;
}

int EvaluateLoop(const vixen::Board &board)
{
    int score = board.GetMaterialBalance();

    unsigned i = 0;
    for (const char piece : board.GetPieceList())
    {
        if (piece == 'P' || piece == 'N' || piece == 'B' || piece == 'R' ||
            piece == 'p' || piece == 'n' || piece == 'b' || piece == 'r')
            score += vixen::Search::arrayLookUp[GetEvalIndexLoop(piece)][i];
        ++i;
    }

    return board.IsWhiteToMove() ? score : -score;
}

static void BM_EvalIfElse(benchmark::State& state)
{
    vixen::Board board;
    board.SetBoard(vixen::Constants::TESTPOS1);
    for (auto _ : state)
        benchmark::DoNotOptimize(EvaluateIfElse(board));
}
BENCHMARK(BM_EvalIfElse);

static void BM_EvalLoop(benchmark::State& state)
{
    vixen::Board board;
    board.SetBoard(vixen::Constants::TESTPOS1);
    for (auto _ : state)
        benchmark::DoNotOptimize(EvaluateLoop(board));
}
BENCHMARK(BM_EvalLoop);

static void BM_GetLoop(benchmark::State& state)
{
    for (auto _ : state)
       benchmark::DoNotOptimize(GetEvalIndexLoop('r'));
}
BENCHMARK(BM_GetLoop);

static void BM_GetIfElse(benchmark::State& state)
{
    for (auto _ : state)
       benchmark::DoNotOptimize(GetEvalIndexIfElse('r'));
}
BENCHMARK(BM_GetIfElse);

BENCHMARK_MAIN();
