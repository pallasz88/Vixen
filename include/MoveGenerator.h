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

    private:
        MoveGenerator();
        ~MoveGenerator() = default;
        MoveGenerator(MoveGenerator const&) = delete;
        MoveGenerator(MoveGenerator&&) = delete;
        MoveGenerator& operator=(MoveGenerator const&) = delete;

        std::vector<std::string> moveList;
        static MoveGenerator *instance;

        void GeneratePawnMoves();
        void GenerateKnightMoves();
        void PrintMoveList();
    };
}
