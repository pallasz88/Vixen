#include <benchmark/benchmark.h>
#define private public
#include "engine.h"
#undef private
#include "board.h"
#include <iostream>

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
    for (const auto &i : Vixen::evalMap)
    {
        if (i.first == c)
            return i.second;
    }
    return -1;
}

int EvaluateIfElse(const Vixen::Board &board)
{
    int score = board.GetMaterialBalance();

    unsigned i = 0;
    for (const char piece : board.GetPieceList())
    {
        if (piece == 'P' || piece == 'N' || piece == 'B' || piece == 'R' ||
            piece == 'p' || piece == 'n' || piece == 'b' || piece == 'r')
            score += Vixen::Search::arrayLookUp[GetEvalIndexIfElse(piece)][i];
        ++i;
    }

    return board.IsWhiteToMove() ? score : -score;
}

int EvaluateLoop(const Vixen::Board &board)
{
    int score = board.GetMaterialBalance();

    unsigned i = 0;
    for (const char piece : board.GetPieceList())
    {
        if (piece == 'P' || piece == 'N' || piece == 'B' || piece == 'R' ||
            piece == 'p' || piece == 'n' || piece == 'b' || piece == 'r')
            score += Vixen::Search::arrayLookUp[GetEvalIndexLoop(piece)][i];
        ++i;
    }

    return board.IsWhiteToMove() ? score : -score;
}

static void BM_Evaluate(benchmark::State& state)
{
    Vixen::Board board;
    board.SetBoard(Vixen::Constants::TESTPOS1);
    for (auto _ : state)
       benchmark::DoNotOptimize(Vixen::Search::Evaluate(board));
}
BENCHMARK(BM_Evaluate);

static void BM_EvalIfElse(benchmark::State& state)
{
    Vixen::Board board;
    board.SetBoard(Vixen::Constants::TESTPOS1);
    for (auto _ : state)
        benchmark::DoNotOptimize(EvaluateIfElse(board));
}
BENCHMARK(BM_EvalIfElse);

static void BM_EvalLoop(benchmark::State& state)
{
    Vixen::Board board;
    board.SetBoard(Vixen::Constants::TESTPOS1);
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
