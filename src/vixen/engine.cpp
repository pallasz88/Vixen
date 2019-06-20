#include "engine.h"

namespace Vixen::Search
{
    int AlphaBeta(int depth, int alpha, int beta, Board& board)
    {
        MoveGenerator generator = board.CreateGenerator<ALL_MOVE>();
        auto moveList = generator.GetMoveList();
        auto size = generator.GetListSize();

        if (depth == 0)
//            return Evaluate(board);
            return Quiescence(alpha, beta, board);

        board.IsWhiteToMove() ? generator.GenerateMoves<Colors::WHITE, ALL_MOVE>(board)
                              : generator.GenerateMoves<Colors::BLACK, ALL_MOVE>(board);

        for (int i = 0; i < size; ++i)
        {
            if (!board.MakeMove(moveList[i]))
                continue;

            int score = -AlphaBeta(depth - 1, alpha, beta, board);
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

        for (int i = 0; i < size; ++i)
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
        std::array<char, SQUARE_NUMBER> pieceList = board.GetPieceList();
        int value = 0;
        for (const auto &piece : pieceList)
        {
            switch (piece)
            {
                case 'P':
                    value += 100;
                    break;
                case 'N':
                    value += 300;
                    break;
                case 'B':
                    value += 300;
                    break;
                case 'R':
                    value += 500;
                    break;
                case 'Q':
                    value += 900;
                    break;
                case 'p':
                    value += -100;
                    break;
                case 'n':
                    value += -300;
                    break;
                case 'b':
                    value += -300;
                    break;
                case 'r':
                    value += -500;
                    break;
                case 'q':
                    value += -900;
                    break;
                default:
                    break;
            }
        }
        return value;
    }
}