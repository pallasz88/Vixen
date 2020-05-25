#include <iostream>
#include <limits>

#include "engine.h"
#include "move_generator.h"
#include "board.h"


namespace Vixen::Search
{
    std::pair<int, Move> IterativeDeepening(Board &board, SearchInfo &info)
    {
        std::pair<int, Move> result;

        for (int depth = 1; depth <= info.maxDepth; ++depth)
            result = Search::Root(depth, board, info);

        return result;
    }

    std::pair<int, Move> Root(int depth, Board &board, SearchInfo &info)
    {
        const auto &generator = board.CreateGenerator<ALL_MOVE>();
        const auto &moveList  = generator.GetLegalMoveList(board);
        int        alpha      = -std::numeric_limits<int>::max();
        int        beta       = std::numeric_limits<int>::max();
        Move       bestMove{0};
        ++info.nodesCount;

        if (moveList.empty())
            return {-std::numeric_limits<int>::max(), bestMove};

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

        return {alpha, bestMove};
    }

    int NegaMax(int depth, int alpha, int beta, Board &board, SearchInfo &info)
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
                alpha = score; // alpha acts like max in MiniMax

        }

        if (legalMoveCount == 0)
        {
            if (Check::IsInCheck<Colors::WHITE>(board) ||
                Check::IsInCheck<Colors::BLACK>(board))
            {
                return -2999999 + info.currentDepth;
            }

            else
                return 0;
        }

        return alpha;
    }

    int Quiescence(int alpha, int beta, Board &board, SearchInfo &info)
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

    int Evaluate(const Board &board)
    {
        int           score = board.GetMaterialBalance();
        for (unsigned i     = 0; i < Constants::SQUARE_NUMBER; ++i)
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