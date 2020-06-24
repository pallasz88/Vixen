//#include <iostream>
#include "engine.h"

#include <algorithm>

#include "move_generator.h"
#include "uci.h"

namespace Vixen
{
PrincipalVariation Search::pv(megaByte / sizeof(PVEntry));

std::vector<Move> Search::GetPV(int depth, Board &board)
{
    std::vector<Move> moveList;
    int ply = 0;

    for (; ply < depth;)
    {
        const auto bestMove = pv.GetPVEntry(board.GetHash()).move;

        if (bestMove == 0)
            break;

        if (board.MakeMove(bestMove))
        {
            ++ply;
            moveList.emplace_back(bestMove);
        }

        else
            break;
    }

    for (int j = 0; j < ply; ++j)
        board.TakeBack();

    return moveList;
}

Move Search::IterativeDeepening(Board &board, SearchInfo &info)
{
    std::pair<int, Move> result;

    for (int depth = 1; depth <= info.maxDepth; ++depth)
    {
        result = Search::Root(depth, board, info);
        const auto bestLine = GetPV(depth, board);
        Uci::LogUci(info, result, depth, bestLine);
    }

    return result.second;
} 

std::pair<int, Move> Search::Root(int depth, Board &board, SearchInfo &info)
{
    const auto &generator = board.CreateGenerator<ALL_MOVE>();
    auto moveList = generator.GetLegalMoveList(board);
    int alpha = -MATE;
    int beta = MATE;
    Move bestMove{0};
    ++info.nodesCount;

    const auto pvEntry = pv.GetPVEntry(board.GetHash());

    for (auto &move : moveList)
    {
        if (pvEntry.move != 0 && move == pvEntry.move)
        {
            move.SetScore(2000000U);
            continue;
        }

        if ((move.GetMoveType() & CAPTURE) == CAPTURE)
        {
            const auto attacker = board.GetPieceList()[move.GetFromSquare()];
            const auto victim = board.GetPieceList()[move.GetToSquare()];
            move.SetScore(mvvlvaTable[GetPieceIndex(attacker)][GetPieceIndex(victim)] + 1000000U);
        }
    }

    while (!moveList.empty())
    {
        const Move move = PickBest(moveList);
        board.MakeMove(move);
        ++info.currentDepth;
        const int score = -NegaMax(depth - 1, -beta, -alpha, board, info);
        --info.currentDepth;

        if (info.stopped)
            return {score, move};

        board.TakeBack();

        if (score >= beta)
            return {beta, move}; //  fail hard beta-cutoff

        if (score > alpha)
        {
            alpha = score; // alpha acts like max in MiniMax
            bestMove = move;
        }
    }

    if (!info.stopped)
        pv.StorePVEntry(PVEntry{bestMove, board.GetHash()});

    return {alpha, bestMove};
}

int Search::NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info)
{
    if (depth == 0)
        return Quiescence(alpha, beta, board, info);

    if (board.IsRepetition() || board.GetFiftyMoveCounter() >= 100)
        return 0;

    ++info.nodesCount;

    const auto &generator = board.CreateGenerator<ALL_MOVE>();
    auto moveList = generator.GetMoveList();
    unsigned legalMoveCount = 0;

    for (auto &move : moveList)
    {
        if ((move.GetMoveType() & CAPTURE) == CAPTURE)
        {
            const auto attacker = board.GetPieceList()[move.GetFromSquare()];
            const auto victim = board.GetPieceList()[move.GetToSquare()];
            move.SetScore(mvvlvaTable[GetPieceIndex(attacker)][GetPieceIndex(victim)] + 1000000U);
        }
    }

    while (!moveList.empty())
    {
        const Move move = PickBest(moveList);
        if (!board.MakeMove(move))
            continue;

        ++legalMoveCount;
        ++info.currentDepth;
        const int score = -NegaMax(depth - 1, -beta, -alpha, board, info);
        --info.currentDepth;
        board.TakeBack();

        if (score >= beta)
            return beta; //  fail hard beta-cutoff

        if (score > alpha)
        {
            alpha = score; // alpha acts like max in MiniMax
            pv.StorePVEntry(PVEntry{move, board.GetHash()});
        }
    }

    if (legalMoveCount == 0)
    {
        if (Check::IsInCheck<Colors::WHITE>(board) || Check::IsInCheck<Colors::BLACK>(board))
            return -MATE + info.currentDepth;

        else
            return STALE_MATE;
    }

    return alpha;
}

int Search::Quiescence(int alpha, int beta, Board &board, SearchInfo &info)
{
    ++info.nodesCount;
    int stand_pat = Evaluate(board);

    if (stand_pat >= beta)
        return beta;

    if (alpha < stand_pat)
        alpha = stand_pat;

    const auto &generator = board.CreateGenerator<CAPTURE>();
    auto moveList = generator.GetMoveList();

    for (auto &move : moveList)
    {
        if ((move.GetMoveType() & CAPTURE) == CAPTURE)
        {
            const auto attacker = board.GetPieceList()[move.GetFromSquare()];
            const auto victim = board.GetPieceList()[move.GetToSquare()];
            move.SetScore(mvvlvaTable[GetPieceIndex(attacker)][GetPieceIndex(victim)] + 1000000U);
        }
    }

    while (!moveList.empty())
    {
        const Move move = PickBest(moveList);
        if (!board.MakeMove(move))
            continue;

        const int score = -Quiescence(-beta, -alpha, board, info);
        board.TakeBack();

        if (score >= beta)
            return beta;

        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

int Search::Evaluate(const Board &board)
{
    int score = board.GetMaterialBalance();

    for (unsigned i = 0; i < Constants::SQUARE_NUMBER; ++i)
    {
        const char piece = board.GetPieceList()[i];

        if (int index = GetEvalIndex(piece); index != -1)
        {
            if (IsWhitePiece(piece))
                score += arrayLookUp[static_cast<unsigned>(index)][i];
            else
                score -= arrayLookUp[static_cast<unsigned>(index)][i];
        }
    }
    return board.IsWhiteToMove() ? score : -score;
}
} // namespace Vixen
