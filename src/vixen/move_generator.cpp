#include "move_generator.h"
#include "board.h"

#include <iostream>

namespace Vixen
{
    MoveGenerator::MoveGenerator(const Board &board)
    {
        BitBoards bitBoards = board.GetBitBoards();
        GeneratePawnMoves(bitBoards);
        GenerateKnightMoves(bitBoards);
        GenerateBishopMoves(bitBoards);
        GenerateRookMoves(bitBoards);
        GenerateQueenMoves(bitBoards);
        GenerateKingMoves(bitBoards);
#ifdef DEBUG
        PrintMoveList();
#endif // DEBUG
    }

    void MoveGenerator::GeneratePawnMoves(const BitBoards &board)
    {

    }

    void MoveGenerator::GenerateKnightMoves(const BitBoards &board)
    {

    }

    void MoveGenerator::GenerateBishopMoves(const BitBoards &board)
    {

    }

    void MoveGenerator::GenerateRookMoves(const BitBoards &board)
    {

    }

    void MoveGenerator::GenerateQueenMoves(const BitBoards &board)
    {

    }

    void MoveGenerator::GenerateKingMoves(const BitBoards &board)
    {

    }

    void MoveGenerator::PrintMoveList()
    {
        for (auto &x : moveList)
            std::cout << x << std::endl;
    }
}