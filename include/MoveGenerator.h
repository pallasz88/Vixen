#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <vector>
#include <string>

namespace Vixen
{
    class DLL_EXPORT MoveGenerator
    {
    public:
        static MoveGenerator* GetInstance();
        void CleanUp();
        MoveGenerator(MoveGenerator const&) = delete;
        MoveGenerator& operator=(MoveGenerator const&) = delete;

    private:
        std::vector<std::string> moveList;
        static MoveGenerator *instance;

        MoveGenerator();
        void GeneratePawnMoves();
        void GenerateKnightMoves();
        void PrintMoveList();
    };
}
