#pragma once

#include <array>
#include <string>
#include "anti_slider.h"
#include "slider.h"
#include "board.h"

namespace Vixen
{
    /**
     * Generates all pseudo-legal moves from Board class position.
     */
    class VIXEN_API MoveGenerator
    {
    public:

        /**
         * Returns pseudo-legal move list.
         * @return moveList
         */
        [[nodiscard]] constexpr auto GetMoveList() const
        { return moveList; }

        /**
         * Returns pseudo-legal move list size.
         * @return moveList
         */
        [[nodiscard]] constexpr auto GetListSize() const
        { return size; }

        /**
         * Fills moveList by generating all pseudo moves.
         * @tparam sideToMove
         * @param board
         */
        template<Colors sideToMove>
        void GenerateAllMoves(const Board &board);

    private:

        std::array<Move, 300> moveList;

        int size = 0;

        template<Colors sideToMove>
        void GenerateQuietMoves(const Board &board);

        template<Colors sideToMove>
        void GenerateCaptureMoves(const Board &board);

        template<Slider slider, uint8_t moveType>
        void GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets);

        template <uint8_t moveType>
        constexpr void GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard);

        constexpr void GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType);

        constexpr void GeneratePawnPromotionMoves(int offset, BitBoard promotion);

        constexpr void GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion);

        template<Colors sideToMove>
        constexpr void GenerateCastlingMoves(const Board &board);
    };

    namespace Check
    {
        template<Colors sideToMove>
        inline constexpr bool IsSquareAttacked(int square, const Board &board)
        {
            auto blockers = ~board.GetBitBoard(' ');
            auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard('p') : board.GetBitBoard('P');
            auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard('n') : board.GetBitBoard('N');
            auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard('b') : board.GetBitBoard('B');
            auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard('r') : board.GetBitBoard('R');
            auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard('q') : board.GetBitBoard('Q');
            auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard('k') : board.GetBitBoard('K');

            return AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)][square] & pawns ||
                   AntSliderUtils::knightAttack[square] & knights ||
                   SliderUtils::GetBishopAttack(square, blockers) & (bishops | queens) ||
                   SliderUtils::GetRookAttack(square, blockers) & (rooks | queens) ||
                   AntSliderUtils::kingAttack[square] & kings;
        }

        /**
         * This function returns if king is in check.
         * @tparam sideToMove
         * @param board
         * @return Check on board
         */
        template<Colors sideToMove>
        inline constexpr bool IsInCheck(const Board &board)
        {
            auto kingBoard = sideToMove == Colors::WHITE ? board.GetBitBoard('K') : board.GetBitBoard('k');
            int kingSquare = TrailingZeroCount(kingBoard);
            return IsSquareAttacked<sideToMove>(kingSquare, board);
        }
    }


    namespace Test
    {
        extern BitBoard leafs;

        /**
         * Helper function to validate move generation:
         *  - Perft method: https://www.chessprogramming.org/Perft
         *  - Perft results: https://www.chessprogramming.org/Perft_Results
         * @param depth
         * @param board
         * @return visited node number
         */
        VIXEN_API BitBoard PerftTest(int depth, Board &board);

        VIXEN_API BitBoard Perft(int depth, Board &board);
    }
}
