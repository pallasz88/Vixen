#pragma once

#include <vector>
#include <string>
#include "anti_slider.h"
#include "slider.h"

namespace Vixen
{
    class Board;

    class VIXEN_API MoveGenerator
    {
    public:

        void PrintMoveList();

        auto GetMoveList() const
        { return moveList; }

        static std::vector<Move> GetAllMoves(const Board &board);

        template<Colors sideToMove>
        void GenerateAllMoves(const Vixen::Board &board);

        static BitBoard Perft(int depth, Board &board);

        static BitBoard PerftTest(int depth, Board &board);

        template<Colors sideToMove>
        static bool IsInCheck(const BitBoards &bitBoards);

    private:

        std::vector<Move> moveList;

        template<Colors sideToMove>
        void GenerateQuietMoves(const Board &board);

        template<Colors sideToMove>
        void GenerateCaptureMoves(const Board &board);

        template<Slider slider>
        void GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets, uint8_t moveType);

        void GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard,
                                     uint8_t moveType);

        void GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType);

        void GeneratePawnPromotionMoves(int offset, BitBoard promotion);

        void GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion);

        template<Colors sideToMove>
        void GenerateCastlingMoves(const Board &board);

        Move CreateMove(int from, int to, uint8_t moveType);

        int GetPosition(BitBoard &bitBoard) const;

        template<Colors sideToMove>
        static bool IsSquareAttacked(int square, const BitBoards &bitBoards);
    };

    template<Colors sideToMove>
    bool MoveGenerator::IsSquareAttacked(int square, const BitBoards &bitBoards)
    {
        auto blockers = ~bitBoards.at(' ');
        auto pawns = sideToMove == Colors::WHITE ? bitBoards.at('p') : bitBoards.at('P');
        auto knights = sideToMove == Colors::WHITE ? bitBoards.at('n') : bitBoards.at('N');
        auto bishops = sideToMove == Colors::WHITE ? bitBoards.at('b') : bitBoards.at('B');
        auto rooks = sideToMove == Colors::WHITE ? bitBoards.at('r') : bitBoards.at('R');
        auto queens = sideToMove == Colors::WHITE ? bitBoards.at('q') : bitBoards.at('Q');
        auto kings = sideToMove == Colors::WHITE ? bitBoards.at('k') : bitBoards.at('K');

        return pawnAttack[static_cast<int>(sideToMove)][square] & pawns ||
               knightAttack[square] & knights ||
               GetBishopAttack(square, blockers) & (bishops | queens) ||
               GetRookAttack(square, blockers) & (rooks | queens) ||
               kingAttack[square] & kings;
    }

    template<Colors sideToMove>
    bool MoveGenerator::IsInCheck(const BitBoards &bitBoards)
    {
        auto kingBoard = sideToMove == Colors::WHITE ? bitBoards.at('K') : bitBoards.at('k');
        int kingSquare = TrailingZeroCount(kingBoard);
        return IsSquareAttacked<sideToMove>(kingSquare, bitBoards);
    }
}
