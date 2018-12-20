#pragma once

#include <vector>
#include <string>
#include "slider.h"

namespace Vixen
{
    class Board;

    class VIXEN_API MoveGenerator
    {
    public:

        explicit MoveGenerator(const Board &);

        void PrintMoveList();

        inline std::vector<std::string> GetMoveList() const
        { return moveList; }

    private:

        std::vector<std::string> moveList;

        void GenerateAllMoves(const BitBoards &bitBoards);

        template <Slider slider>
        void GenerateSliderMoves(const BitBoards &bitBoards, BitBoard targets);

        void GenerateQuietMoves(const BitBoards &bitBoards, BitBoard targets);

        void GenerateCaptureMoves(const BitBoards &bitBoards, BitBoard targets);

    };
}
