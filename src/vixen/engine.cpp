#include <iostream>
#include "engine.h"
#include "move_generator.h"
#include "board.h"

namespace Vixen::Search
{
    std::pair<int, Move> Root(int depth, Board &board)
    {
        if (depth == 0)
            return {};

        const auto generator = board.CreateGenerator<ALL_MOVE>();
        int alpha = -100000;
        int beta = 100000;
        int score{0};
        Move bestMove{0};

        for (unsigned i = 0; i < generator.GetListSize(); ++i)
        {
            const auto move = generator.GetMoveList()[i];
            if (!board.MakeMove(move))
                continue;

            bestMove == 0 ? bestMove = move : bestMove;
            score = -NegaMax(depth - 1, -beta, -alpha, board);
            board.TakeBack();

            if (score >= beta)
                return {beta, move}; //  fail hard beta-cutoff

            if (score > alpha)
            {
                alpha = score; // alpha acts like max in MiniMax
                bestMove = move;
            }
        }
        return {alpha, bestMove};
    }

    int NegaMax(int depth, int alpha, int beta, Board& board)
    {
        if (depth == 0)
            return Quiescence(alpha, beta, board);

        MoveGenerator generator = board.CreateGenerator<ALL_MOVE>();
        auto moveList = generator.GetMoveList();
        auto size = generator.GetListSize();

        board.IsWhiteToMove() ? generator.GenerateMoves<Colors::WHITE, ALL_MOVE>(board)
                              : generator.GenerateMoves<Colors::BLACK, ALL_MOVE>(board);

        for (unsigned i = 0; i < size; ++i)
        {
            if (!board.MakeMove(moveList[i]))
                continue;

            int score = -NegaMax(depth - 1, -beta, -alpha, board);
            board.TakeBack();

            if (score >= beta)
                return beta; //  fail hard beta-cutoff

            if (score > alpha)
                alpha = score; // alpha acts like max in MiniMax

        }
        return alpha;
    }

    int Quiescence(int alpha, int beta, Board& board)
    {
        int stand_pat = Evaluate(board);
        if( stand_pat >= beta )
            return beta;
        if( alpha < stand_pat )
            alpha = stand_pat;

        MoveGenerator generator = board.CreateGenerator<CAPTURE>();
        auto moveList = generator.GetMoveList();
        auto size = generator.GetListSize();

        for (unsigned i = 0; i < size; ++i)
        {
            if (!board.MakeMove(moveList[i]))
                continue;

            int score = -Quiescence( -beta, -alpha, board);
            board.TakeBack();

            if( score >= beta )
                return beta;
            if( score > alpha )
                alpha = score;
        }
        return alpha;
    }

    int Evaluate(const Board &board)
    {
        return board.GetMaterialBalance();
    }
}