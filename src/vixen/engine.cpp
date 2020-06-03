#include <iostream>

#include "engine.h"
#include "move_generator.h"
#include "uci.h"


namespace Vixen
{
    PrincipalVariation Search::pv;

    std::vector<Move> Search::GetPV(int depth, Board &board)
    {
        std::vector<Move> moveList;
        int               ply = 0;

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
        const auto &moveList  = generator.GetLegalMoveList(board);
        int        alpha      = -MATE;
        int        beta       = MATE;
        Move       bestMove{0};
        ++info.nodesCount;

        for (const Move &move : moveList)
        {
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
                alpha    = score; // alpha acts like max in MiniMax
                bestMove = move;
            }
        }

        if (bestMove == 0)
            bestMove = moveList[0];

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

        const auto &generator     = board.CreateGenerator<ALL_MOVE>();
        const auto &moveList      = generator.GetMoveList();
        const auto size           = generator.GetListSize();
        unsigned   legalMoveCount = 0;

        for (unsigned i = 0; i < size; ++i)
        {
            if (!board.MakeMove(moveList[i]))
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
                pv.StorePVEntry(PVEntry{moveList[i], board.GetHash()});
            }

        }

        if (legalMoveCount == 0)
        {
            if (Check::IsInCheck<Colors::WHITE>(board) ||
                Check::IsInCheck<Colors::BLACK>(board))
            {
                return -MATE + info.currentDepth;
            }

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
        const auto &moveList  = generator.GetMoveList();
        const auto size       = generator.GetListSize();

        for (unsigned i = 0; i < size; ++i)
        {
            if (!board.MakeMove(moveList[i]))
                continue;

            int score = -Quiescence(-beta, -alpha, board, info);
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
            if (board.GetPieceList()[i] == 'P')
                score += pawnTable[i];

            else if (board.GetPieceList()[i] == 'p')
                score -= PawnTable[i];

            else if (board.GetPieceList()[i] == 'N')
                score += knightTable[i];

            else if (board.GetPieceList()[i] == 'n')
                score -= KnightTable[i];

            else if (board.GetPieceList()[i] == 'B')
                score += bishopTable[i];

            else if (board.GetPieceList()[i] == 'b')
                score -= BishopTable[i];

            else if (board.GetPieceList()[i] == 'R')
                score += rookTable[i];

            else if (board.GetPieceList()[i] == 'r')
                score -= RookTable[i];
        }
        return board.IsWhiteToMove() ? score : -score;
    }
}