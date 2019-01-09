#pragma once

#include <vector>
#include <string>
#include "anti_slider.h"

namespace Vixen
{
    class Board;
    
    class SliderAttacks;

    class VIXEN_API MoveGenerator
    {
    public:

        void PrintMoveList();

        inline auto GetMoveList() const
        { return moveList; }

        static std::vector<Move> GetAllMoves(const Board &board);

        template<Colors sideToMove>
        void GenerateAllMoves(const Vixen::Board &board);

        static BitBoard Perft(int depth, Board &board);

        static BitBoard PerftTest(int depth, Board &board);

        template<Colors sideToMove>
        static bool IsInCheck(const BitBoards &bitBoards, const SliderAttacks &sliders);

    private:

        std::vector<Move> moveList;

        template<Colors sideToMove>
        void GenerateQuietMoves(const Board &board, BitBoard targets);

        template<Colors sideToMove>
        void GenerateCaptureMoves(const Board &board, BitBoard targets);

        template<Slider slider>
        void GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets, uint8_t moveType,
                                 SliderAttacks sliders);

        void GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard,
                                     uint8_t moveType);

        void GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType);

        void GeneratePawnPromotionMoves(int offset, BitBoard promotion);

        void GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion);

        template<Colors sideToMove>
        void GenerateCastlingMoves(const BitBoards &bitBoards, int castlingRights, const SliderAttacks &sliders);

        Move CreateMove(uint8_t from, uint8_t to, uint8_t moveType);

        uint8_t GetPosition(BitBoard &bitBoard) const;

        template<Colors sideToMove>
        static bool IsSquareAttacked(int square, const BitBoards &bitBoards, SliderAttacks sliders);
    };
}
