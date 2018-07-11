#pragma once

#include <vector>
#include <string>
#include "defs.h"

namespace Vixen
{
    class Board;

    class VIXEN_API MoveGenerator
    {
    public:
        MoveGenerator(const Board&);
        ~MoveGenerator() = default;
        MoveGenerator(MoveGenerator const&) = default;
        MoveGenerator(MoveGenerator&&) = default;

        void PrintMoveList();
        const std::vector<std::string>& GetMoveList() const { return moveList; }

    private:
        std::vector<std::string> moveList;

        void GeneratePawnMoves(const Board&);
        void GenerateKnightMoves(const Board&);
        void GenerateBishopMoves(const Board&);
        void GenerateRookMoves(const Board&);
        void GenerateQueenMoves(const Board&);
        void GenerateKingMoves(const Board&);
    };
}
