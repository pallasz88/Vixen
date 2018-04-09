#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <vector>
#include <string>
#include <boost\thread.hpp>

namespace Vixen
{
    class DLL_EXPORT MoveGenerator
    {
    public:
        static MoveGenerator* GetInstance();
        void CleanUp();
        void PrintMoveList();
        const std::vector<std::string>& GetMoveList() const { return moveList; }
        void ResetMoveList() noexcept { moveList.clear(); }

    private:
        MoveGenerator();
        ~MoveGenerator() = default;
        MoveGenerator(MoveGenerator const&) = delete;
        MoveGenerator(MoveGenerator&&) = delete;
        MoveGenerator& operator=(MoveGenerator const&) = delete;

        std::vector<std::string> moveList;
        static MoveGenerator *instance;
        boost::mutex mutex;

        void GeneratePawnMoves();
        void GenerateKnightMoves();
    };
}
