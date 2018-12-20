#include "move_generator.h"
#include "board.h"

#include <iostream>

namespace Vixen
{
    MoveGenerator::MoveGenerator(const Board &board)
    {
        auto bitBoards = board.GetBitBoards();
        GenerateAllMoves(bitBoards);
#ifdef DEBUG
        PrintMoveList();
#endif // DEBUG
    }

    void MoveGenerator::PrintMoveList()
    {
        for (auto &move : moveList)
            std::cout << move << std::endl;
    }

    void MoveGenerator::GenerateAllMoves(const BitBoards &bitBoards)
    {
        auto targets = EMPTY_BOARD; // TODO: calculate targets for below functions to work with
        // TODO: targets are enemy pieces in case of CAPTURE moves and empty square if QUIET move is being calculated
        GenerateQuietMoves(bitBoards, targets);
        GenerateCaptureMoves(bitBoards, targets);
    }

    template<Slider slider>
    void MoveGenerator::GenerateSliderMoves(const BitBoards &bitBoards, BitBoard targets)
    {
        SliderAttacks sliders;
        BitBoard blockers = ~bitBoards.at(' ');
        unsigned position = D8; // TODO: Get position for all sliding piece types
        BitBoard attacks =
                slider == Slider::BISHOP ? sliders.GetBishopAttack(position, blockers) :
                slider == Slider::QUEEN ? sliders.GetQueenAttack(position, blockers) :
                sliders.GetRookAttack(position, blockers); // TODO: calculate attacks for all pieces
        attacks &= targets;
        std::cout << attacks << std::endl;
    }

    void MoveGenerator::GenerateQuietMoves(const BitBoards &bitBoards, BitBoard targets)
    {
        GenerateSliderMoves<Slider::BISHOP>(bitBoards, targets);
        GenerateSliderMoves<Slider::ROOK>(bitBoards, targets);
        GenerateSliderMoves<Slider::QUEEN>(bitBoards, targets);
    }

    void MoveGenerator::GenerateCaptureMoves(const BitBoards &bitBoards, BitBoard targets)
    {
        GenerateSliderMoves<Slider::BISHOP>(bitBoards, targets);
        GenerateSliderMoves<Slider::ROOK>(bitBoards, targets);
        GenerateSliderMoves<Slider::QUEEN>(bitBoards, targets);
    }
}
