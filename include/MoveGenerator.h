#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <vector>
#include <string>
#include <boost\thread.hpp>

namespace Vixen
{
    class Board;

    class DLL_EXPORT MoveGenerator
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
        static MoveGenerator *instance;
        boost::mutex mutex;

        void GeneratePawnMoves(const Board&);
        void GenerateKnightMoves(const Board&);
        void GenerateBishopMoves(const Board&);
        void GenerateRookMoves(const Board&);
        void GenerateQueenMoves(const Board&);
    };
}
