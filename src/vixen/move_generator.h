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

        explicit MoveGenerator(const Board &);

        void PrintMoveList();

        inline auto GetMoveList() const
        { return moveList; }

    private:

        std::vector<std::string> moveList;

        template<Colors sideTomMove>
        void GenerateAllMoves(const BitBoards &bitBoards, BitBoard enPassant);

        template<Slider slider>
        void GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets);

        template<Colors sideToMove>
        void GenerateQuietMoves(const BitBoards &bitBoards, BitBoard targets);

        template<Colors sideToMove>
        void GenerateCaptureMoves(const BitBoards &bitBoards, BitBoard targets, BitBoard enPassant);

        std::string MakeMove(unsigned int from, unsigned to);

        unsigned GetPosition(BitBoard &bitBoard) const;

        void GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard);

        void GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed);
    };
}
