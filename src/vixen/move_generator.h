#pragma once

#include "defs.h"

namespace Vixen
{
    class Board;

    class VIXEN_API MoveGenerator
    {
    public:

        explicit MoveGenerator(const Board &);

        void PrintMoveList();

        std::vector<std::string> GetMoveList() const
        { return moveList; }

    private:

        std::vector<std::string> moveList;

        void GeneratePawnMoves(const BitBoards &);

        void GenerateKnightMoves(const BitBoards &);

        void GenerateBishopMoves(const BitBoards &);

        void GenerateRookMoves(const BitBoards &);

        void GenerateQueenMoves(const BitBoards &);

        void GenerateKingMoves(const BitBoards &);
    };
}
