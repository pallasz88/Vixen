#pragma once

#include <array>
#include <string>
#include "anti_slider.h"
#include "slider.h"

namespace Vixen
{
    class Board;

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

        template<Slider slider>
        void GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets, uint8_t moveType);

        constexpr void GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard,
                                               uint8_t moveType);

        constexpr void GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType);

        constexpr void GeneratePawnPromotionMoves(int offset, BitBoard promotion);

        constexpr void GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion);

        template<Colors sideToMove>
        constexpr void GenerateCastlingMoves(const Board &board);
    };

    template<Colors sideToMove>
    void MoveGenerator::GenerateAllMoves(const Vixen::Board &board)
    {
        GenerateCaptureMoves<sideToMove>(board);
        GenerateQuietMoves<sideToMove>(board);
    }

    namespace Check
    {
        template<Colors sideToMove>
        inline constexpr bool IsSquareAttacked(int square, const BitBoards &bitBoards)
        {
            auto blockers = ~bitBoards.at(' ');
            auto pawns = sideToMove == Colors::WHITE ? bitBoards.at('p') : bitBoards.at('P');
            auto knights = sideToMove == Colors::WHITE ? bitBoards.at('n') : bitBoards.at('N');
            auto bishops = sideToMove == Colors::WHITE ? bitBoards.at('b') : bitBoards.at('B');
            auto rooks = sideToMove == Colors::WHITE ? bitBoards.at('r') : bitBoards.at('R');
            auto queens = sideToMove == Colors::WHITE ? bitBoards.at('q') : bitBoards.at('Q');
            auto kings = sideToMove == Colors::WHITE ? bitBoards.at('k') : bitBoards.at('K');

            return AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)][square] & pawns ||
                   AntSliderUtils::knightAttack[square] & knights ||
                   SliderUtils::GetBishopAttack(square, blockers) & (bishops | queens) ||
                   SliderUtils::GetRookAttack(square, blockers) & (rooks | queens) ||
                   AntSliderUtils::kingAttack[square] & kings;
        }

        /**
         * This function returns if king is in check.
         * @tparam sideToMove
         * @param bitBoards
         * @return Check on board
         */
        template<Colors sideToMove>
        inline constexpr bool IsInCheck(const BitBoards &bitBoards)
        {
            auto kingBoard = sideToMove == Colors::WHITE ? bitBoards.at('K') : bitBoards.at('k');
            int kingSquare = TrailingZeroCount(kingBoard);
            return IsSquareAttacked<sideToMove>(kingSquare, bitBoards);
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
