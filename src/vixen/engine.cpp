#include "engine.h"

#include <algorithm>
#include <iostream>

#include "move_generator.h"
#include "uci.h"

namespace Vixen
{
PrincipalVariation Search::pv(megaByte / sizeof(PVEntry));

FixedList<Move> Search::GetPV(int depth, Board &board)
{
    FixedList<Move> moveList;
    int ply = 0;

    for (; ply < depth;)
    {
        const auto bestMove = pv.GetPVEntry(board.GetHash()).move;

        if (bestMove == 0U)
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

void Search::IterativeDeepening(Board &board, SearchInfo &info)
{
    info.stopped = false;
    info.startTime = std::chrono::high_resolution_clock::now();
    info.timeIndex = static_cast<size_t>(!board.IsWhiteToMove());
    std::pair<int, Move> result;
    Move bestMove{};

    for (int depth = 1; depth <= info.maxDepth; ++depth)
    {
        result = Search::Root(depth, board, info);
        const auto bestLine = GetPV(depth, board);
        bestMove = static_cast<bool>(bestLine[0]) ? bestLine[0] : bestMove;
        if (info.stopped)
        {
            info.stopped = false;
            std::cout << "bestmove " << bestMove << std::endl;
            return;
        }
        Uci::LogUci(info, result, depth, bestLine);
    }
    std::cout << "bestmove " << bestMove << std::endl;
}

std::pair<int, Move> Search::Root(int depth, Board &board, SearchInfo &info)
{
    auto moveList = board.GetMoveList<ALL_MOVE>();
    int alpha = -MATE;
    int beta = MATE;
    Move bestMove{0U};
    ++info.nodesCount;

    const auto pvEntry = pv.GetPVEntry(board.GetHash());

    const bool inCheck = Check::IsInCheck<Colors::WHITE>(board) || Check::IsInCheck<Colors::BLACK>(board);
    if (inCheck)
        ++depth;

    for (auto &move : moveList)
    {
        if (pvEntry.move != 0U && move == pvEntry.move)
        {
            move.SetScore(2000000U);
            continue;
        }

        if ((move.GetMoveType() & CAPTURE) == CAPTURE)
        {
            const auto attacker = board.GetPieceList()[move.GetFromSquare()];
            const auto victim = board.GetPieceList()[move.GetToSquare()];
            const auto attackerIndex = static_cast<size_t>(GetPieceIndex(static_cast<unsigned char>(attacker)));
            const auto victimIndex = static_cast<size_t>(GetPieceIndex(static_cast<unsigned char>(victim)));
            if (attackerIndex < 13U && victimIndex < 13U)
                move.SetScore(mvvlvaTable[attackerIndex][victimIndex] + 1000000U);
        }

        else if (board.GetKiller(depth, 1) == move)
            move.SetScore(900000U);

        else if (board.GetKiller(depth, 0) == move)
            move.SetScore(800000U);

        else
            move.SetScore(board.GetHistoryValue(move.GetFromSquare(), move.GetToSquare()));
    }

    for (auto it = begin(moveList); it != end(moveList); ++it)
    {
        const Move &move = PickBest(it, end(moveList));
        if (!board.MakeMove(move))
            continue;

        ++info.currentDepth;
        const int score = -NegaMax(depth - 1, -beta, -alpha, board, info);
        --info.currentDepth;

        board.TakeBack();

        if (info.stopped)
            return {score, move};

        if (score >= beta)
        {
            if ((move.GetMoveType() & CAPTURE) != CAPTURE)
                board.UpdateKillers(move, depth);

            return {beta, move}; //  fail hard beta-cutoff
        }

        if (score > alpha)
        {
            if ((move.GetMoveType() & CAPTURE) != CAPTURE)
                board.IncreaseHistoryValue(depth, move.GetFromSquare(), move.GetToSquare());

            alpha = score; // alpha acts like max in MiniMax
            bestMove = move;
        }
    }

    if (!info.stopped)
        pv.StorePVEntry(PVEntry{bestMove, board.GetHash()});

    return {alpha, bestMove};
}

void CheckTime(SearchInfo &info)
{
    info.endTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> duration = (info.endTime - info.startTime) * 1000;
    double allocatedTime = info.moveTime;
    if (info.isTimeSet)
        if (info.time[info.timeIndex] != -1)
            allocatedTime = static_cast<double>(info.time[info.timeIndex]) / 30. - 50;

    if (duration.count() >= allocatedTime)
        info.stopped = true;
}

int Search::NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info)
{
    if (depth <= 0)
        return Quiescence(alpha, beta, board, info);

    ++info.nodesCount;

    if (board.IsRepetition() || board.GetFiftyMoveCounter() >= 100)
        return 0;

    if (info.isTimeSet && !(info.nodesCount & 2047))
        CheckTime(info);

    const bool inCheck = Check::IsInCheck<Colors::WHITE>(board) || Check::IsInCheck<Colors::BLACK>(board);
    if (inCheck)
        ++depth;

    if (!inCheck && board.HasHeavyPieces())
    {
        board.MakeNullMove();
        const auto val = -NegaMax(depth - 4, -beta, -beta + 1, board, info);
        board.TakeNullMove();
        if (info.stopped)
            return 0;

        if (val >= beta)
            return beta;
    }

    auto moveList = board.GetMoveList<ALL_MOVE>();
    unsigned legalMoveCount = 0;

    for (auto &move : moveList)
    {
        if ((move.GetMoveType() & CAPTURE) == CAPTURE)
        {
            const auto attacker = board.GetPieceList()[move.GetFromSquare()];
            const auto victim = board.GetPieceList()[move.GetToSquare()];
            const auto attackerIndex = static_cast<size_t>(GetPieceIndex(static_cast<unsigned char>(attacker)));
            const auto victimIndex = static_cast<size_t>(GetPieceIndex(static_cast<unsigned char>(victim)));
            if (attackerIndex < 13U && victimIndex < 13U)
                move.SetScore(mvvlvaTable[attackerIndex][victimIndex] + 1000000U);
        }

        else if (board.GetKiller(depth, 1) == move)
            move.SetScore(900000U);

        else if (board.GetKiller(depth, 0) == move)
            move.SetScore(800000U);

        else
            move.SetScore(board.GetHistoryValue(move.GetFromSquare(), move.GetToSquare()));
    }

    for (auto it = begin(moveList); it != end(moveList); ++it)
    {
        const Move &move = PickBest(it, end(moveList));

        if (!board.MakeMove(move))
            continue;

        ++legalMoveCount;
        ++info.currentDepth;
        const int score = -NegaMax(depth - 1, -beta, -alpha, board, info);
        --info.currentDepth;
        board.TakeBack();

        if (info.stopped)
            return 0;

        if (score >= beta)
        {
            if ((move.GetMoveType() & CAPTURE) != CAPTURE)
                board.UpdateKillers(move, depth);

            return beta; //  fail hard beta-cutoff
        }

        if (score > alpha)
        {
            if ((move.GetMoveType() & CAPTURE) != CAPTURE)
                board.IncreaseHistoryValue(depth, move.GetFromSquare(), move.GetToSquare());

            alpha = score; // alpha acts like max in MiniMax
            pv.StorePVEntry(PVEntry{move, board.GetHash()});
        }
    }

    if (legalMoveCount == 0)
    {
        if (inCheck)
            return -MATE + info.currentDepth;

        else
            return STALE_MATE;
    }

    return alpha;
}

int Search::Quiescence(int alpha, int beta, Board &board, SearchInfo &info)
{
    if (info.isTimeSet && !(info.nodesCount & 2047))
        CheckTime(info);

    ++info.nodesCount;
    int stand_pat = Evaluate(board);

    if (stand_pat >= beta)
        return beta;

    if (alpha < stand_pat)
        alpha = stand_pat;

    auto moveList = board.GetMoveList<CAPTURE>();

    for (auto &move : moveList)
    {
        if ((move.GetMoveType() & CAPTURE) == CAPTURE)
        {
            const auto attacker = board.GetPieceList()[move.GetFromSquare()];
            const auto victim = board.GetPieceList()[move.GetToSquare()];
            const auto attackerIndex = static_cast<size_t>(GetPieceIndex(static_cast<unsigned char>(attacker)));
            const auto victimIndex = static_cast<size_t>(GetPieceIndex(static_cast<unsigned char>(victim)));
            if (attackerIndex < 13U && victimIndex < 13U)
                move.SetScore(mvvlvaTable[attackerIndex][victimIndex] + 1000000U);
        }
    }

    for (auto it = begin(moveList); it != end(moveList); ++it)
    {
        const Move &move = PickBest(it, end(moveList));

        if (!board.MakeMove(move))
            continue;

        const int score = -Quiescence(-beta, -alpha, board, info);
        board.TakeBack();
        if (info.stopped)
            return 0;

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

    unsigned i = 0;
    for (const char piece : board.GetPieceList())
    {
        if (piece == 'P' || piece == 'N' || piece == 'B' || piece == 'R' || piece == 'p' || piece == 'n' ||
            piece == 'b' || piece == 'r')
            score += arrayLookUp[static_cast<std::size_t>(GetEvalIndex(piece))][i];
        ++i;
    }

    return board.IsWhiteToMove() ? score : -score;
}
} // namespace Vixen
