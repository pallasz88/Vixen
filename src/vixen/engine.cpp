#include "engine.hpp"

#include <iostream>
#include <utility>

#include "board.hpp"
#include "defs.hpp"
#include "move.hpp"
#include "move_generator.hpp"
#include "principal_variation.hpp"
#include "uci.hpp"

namespace vixen
{
PrincipalVariation Search::pv(megaByte / sizeof(PVEntry));

FixedList<Move> Search::GetPV(int depth, Board &board)
{
    FixedList<Move> moveList{};
    int ply = 0;

    while (ply < depth)
    {
        if (const auto bestMove = pv.GetPVEntry(board.GetHash()).moveEntry; bestMove != 0U && board.MakeMove(bestMove))
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
    ++info.nodesCount;
    board.ResetKillers();
    board.ResetHistory();

    Move bestMove{};
    for (int depth = 1; depth <= info.maxDepth; ++depth)
    {
        const auto result = Search::Root(depth, board, info);
        const auto bestLine = GetPV(depth, board);
        bestMove = bestLine[0];
        if (info.stopped)
        {
            info.stopped = false;
            break;
        }
        Uci::LogUci(info, result, depth, bestLine);
    }
    std::cout << "bestmove " << bestMove << std::endl;
}

void Search::OrderCapture(const Board &board, Move &move)
{
    if ((move.GetMoveType() & static_cast<uint8_t>(MoveTypes::ENPASSANT)) != static_cast<uint8_t>(MoveTypes::ENPASSANT))
    {
        const auto attackerIndex = board.GetPieceList()[move.GetFromSquare()];
        const auto victimIndex = board.GetPieceList()[move.GetToSquare()];
        move.SetScore(mvvlvaTable[attackerIndex][victimIndex] + 1000000U);
    }
    else
    {
        move.SetScore(1000105U);
    }
}

void CheckTime(SearchInfo &info)
{
    info.endTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> duration = (info.endTime - info.startTime) * 1000;
    double allocatedTime = info.moveTime;
    if (info.isTimeSet && info.time[info.timeIndex] != -1)
        allocatedTime = static_cast<double>(info.time[info.timeIndex]) / 30.;

    if (duration.count() >= allocatedTime)
        info.stopped = true;
}

bool IsTimeCheckNeeded(const SearchInfo &info)
{
    return info.isTimeSet && !(info.nodesCount & 2047);
}

std::pair<int, Move> Search::Root(int depth, Board &board, SearchInfo &info)
{
    if (IsTimeCheckNeeded(info))
        CheckTime(info);

    auto moveList = board.GetMoveList<MoveTypes::ALL_MOVE>();
    int alpha = -MATE;
    int beta = MATE;
    Move bestMove{0U};

    const auto pvEntry = pv.GetPVEntry(board.GetHash());

    const bool inCheck = board.IsInCheck<Colors::WHITE>() || board.IsInCheck<Colors::BLACK>();
    if (inCheck)
        ++depth;

    for (auto &move : moveList)
    {
        if (IsPVMove(pvEntry, move))
            move.SetScore(2000000U);

        else
            OrderNonPVMoves(depth, board, move);
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
        {
            bestMove = vixen::GetBestMove(bestMove, move);
            pv.StorePVEntry(PVEntry{bestMove, board.GetHash()});
            return {score, move};
        }

        if (score >= beta)
        {
            if ((move.GetMoveType() & static_cast<uint8_t>(MoveTypes::CAPTURE)) !=
                static_cast<uint8_t>(MoveTypes::CAPTURE))
                board.UpdateKillers(move, depth);

            return {beta, move}; //  fail hard beta-cutoff
        }

        if (score > alpha)
        {
            if ((move.GetMoveType() & static_cast<uint8_t>(MoveTypes::CAPTURE)) !=
                static_cast<uint8_t>(MoveTypes::CAPTURE))
                board.IncreaseHistoryValue(depth, move.GetFromSquare(), move.GetToSquare());

            alpha = score; // alpha acts like max in MiniMax
            bestMove = move;
            pv.StorePVEntry(PVEntry{bestMove, board.GetHash()});
        }
    }

    return {alpha, bestMove};
}

bool Search::IsPVMove(const PVEntry &pvEntry, const Move &move)
{
    return move == pvEntry.moveEntry;
}

void Search::OrderNonPVMoves(int depth, const Board &board, Move &move)
{
    if ((move.GetMoveType() & static_cast<uint8_t>(MoveTypes::CAPTURE)) == static_cast<uint8_t>(MoveTypes::CAPTURE))
        OrderCapture(board, move);

    else if (board.GetKiller(depth, 1) == move)
        move.SetScore(900000U);

    else if (board.GetKiller(depth, 0) == move)
        move.SetScore(800000U);

    else
        move.SetScore(board.GetHistoryValue(move.GetFromSquare(), move.GetToSquare()));
}

int Search::NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info)
{
    if (depth <= 0)
        return Quiescence(alpha, beta, board, info);

    ++info.nodesCount;

    if (IsTimeCheckNeeded(info))
        CheckTime(info);

    if (board.IsRepetition() || board.GetFiftyMoveCounter() >= 100)
        return 0;

    const bool inCheck = board.IsInCheck<Colors::WHITE>() || board.IsInCheck<Colors::BLACK>();
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

    auto moveList = board.GetMoveList<MoveTypes::ALL_MOVE>();
    unsigned legalMoveCount = 0;

    for (auto &move : moveList)
        OrderNonPVMoves(depth, board, move);

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
            if ((move.GetMoveType() & static_cast<uint8_t>(MoveTypes::CAPTURE)) !=
                static_cast<uint8_t>(MoveTypes::CAPTURE))
                board.UpdateKillers(move, depth);

            return beta; //  fail hard beta-cutoff
        }

        if (score > alpha)
        {
            if ((move.GetMoveType() & static_cast<uint8_t>(MoveTypes::CAPTURE)) !=
                static_cast<uint8_t>(MoveTypes::CAPTURE))
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

    if (IsTimeCheckNeeded(info))
        CheckTime(info);

    ++info.nodesCount;

    int stand_pat = Evaluate(board);

    if (stand_pat >= beta)
        return beta;

    if (alpha < stand_pat)
        alpha = stand_pat;

    auto moveList = board.GetMoveList<MoveTypes::CAPTURE>();

    for (auto &move : moveList)
        OrderCapture(board, move);

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
    auto occupied = ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX);
    const auto &pieceList = board.GetPieceList();

    const int colorMultiplier = 1 | ((board.IsWhiteToMove() - 1) << 1);

    // Unroll loop completely - no branching needed
    while (occupied)
    {
        const auto pos = GetPosition(occupied);
        score += arrayLookUp[pieceList[pos]][pos];
    }

    return score * colorMultiplier;
}
} // namespace vixen
