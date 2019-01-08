#pragma once

#include <vector>
#include <string>
#include "slider.h"
#include "anti_slider.h"

namespace Vixen
{
    class Board;

    class VIXEN_API MoveGenerator
    {
    public:

        void PrintMoveList();

        inline auto GetMoveList() const
        { return moveList; }

        template <Colors sideToMove>
        static bool IsInCheck(const BitBoards &bitBoards, const SliderAttacks &sliders);

        template<Colors sideToMove>
        void GenerateAllMoves(const Vixen::BitBoards &bitBoards, Vixen::BitBoard enPassant, int castlingRights,
                              const SliderAttacks &sliders);

        static BitBoard Perft(int depth, Board &board);

        static BitBoard PerftTest(int depth, Board &board);

    private:

        std::vector<Move> moveList;

        template<Colors sideToMove>
        void GenerateQuietMoves(const BitBoards &bitBoards, BitBoard targets, int castlingRights,
                                const SliderAttacks &sliders);

        template<Colors sideToMove>
        void GenerateCaptureMoves(const BitBoards &bitBoards, BitBoard targets, BitBoard enPassant,
                                  const SliderAttacks &sliders);

        template<Slider slider>
        void GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets, uint8_t moveType,
                                 SliderAttacks sliders);

        void GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard,
                                     uint8_t moveType);

        void GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType);

        void GeneratePawnPromotionMoves(int offset, BitBoard promotion);

        void GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion);

        template <Colors sideToMove>
        void GenerateCastlingMoves(const BitBoards &bitBoards, int castlingRights, const SliderAttacks &sliders);

        Move CreateMove(uint8_t from, uint8_t to, uint8_t moveType);

        uint8_t GetPosition(BitBoard &bitBoard) const;

        template <Colors sideToMove>
        static bool IsSquareAttacked(int square, const BitBoards &bitBoards, SliderAttacks sliders);
    };
}
