#include <benchmark/benchmark.h>
#define private public
#include "engine.h"
#undef private
#include "board.h"
#include <iostream>

constexpr int GetEvalIndex_(char c) noexcept
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

int Evaluate(const Vixen::Board &board)
{
    int score = board.GetMaterialBalance();

    unsigned i = 0;
    for (const char piece : board.GetPieceList())
    {
        if (piece == 'P' || piece == 'N' || piece == 'B' || piece == 'R' ||
            piece == 'p' || piece == 'n' || piece == 'b' || piece == 'r')
            score += Vixen::Search::arrayLookUp[GetEvalIndex_(piece)][i];
        ++i;
    }
    
    return board.IsWhiteToMove() ? score : -score;
}

static void BM_Evaluate(benchmark::State& state)
{
    Vixen::Board board;
    board.SetBoard(Vixen::Constants::TESTPOS1);
    for (auto _ : state)
       Vixen::Search::Evaluate(board); 
}
BENCHMARK(BM_Evaluate);

static void BM_Eval2(benchmark::State& state)
{
    Vixen::Board board;
    board.SetBoard(Vixen::Constants::TESTPOS1);
    for (auto _ : state)
        Evaluate(board);
}
BENCHMARK(BM_Eval2);

BENCHMARK_MAIN();
