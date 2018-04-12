#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "MoveGenerator.h"
#include "Hash.h"
#include <memory>
#include <vector>
#include <string>

namespace Vixen
{
    struct BitBoards
    {
        uint64_t p;
        uint64_t r;
        uint64_t b;
        uint64_t n;
        uint64_t q;
        uint64_t k;
        uint64_t P;
        uint64_t R;
        uint64_t B;
        uint64_t N;
        uint64_t Q;
        uint64_t K;
        uint64_t occupied;
        uint64_t white;
        uint64_t black;
    };

    void InitBitBoards(BitBoards& bitBoards);

    class DLL_EXPORT Board
    {
    public:
        inline BitBoards GetBitBoards() const { return bitBoards; }
        inline MoveGenerator& GetMoveGenerator() const { return *generator; }
        inline bool IsWhiteToMove() const { return whiteToMove; }
        inline const std::string& GetEnPassant() const { return enpassant; }
        void PrintBoard() const;
        void SetBoard(const std::string& fenPosition);
        Board();
        ~Board() = default;
        Board(Board const&) = default;
        Board(Board&&) = default;

    private:
        std::unique_ptr<MoveGenerator> generator;
        std::unique_ptr<Hash> hashBoard;
        BitBoards bitBoards;
        std::string enpassant;
        std::string fenPosition;
        int fullMovesNum;
        bool whiteToMove;
        int castlingRights;
        int fiftyMoves;

        void ParseFenPiecePart(const std::string& splittedFen);
        void ParseSideToMovePart(const std::string& splittedFen);
        void ParseCastlingRightPart(const std::string& splittedFen);
        void SplitFenPosition(std::vector<std::string>& fenParts);

    };
}
