#pragma once

#define DLL_EXPORT __declspec(dllexport)

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

    private:
        static Board* instance;
        Board();
        ~Board();
        BitBoards bitBoards;
        std::string enpassant;
        std::string fenPosition;
        int fullMovesNum;
        bool whiteToMove;
        int castlingRights;
        int fiftyMoves;

        void ParseFenPiecePart(std::string& splittedFen);
        void ParseSideToMovePart(std::string& splittedFen);
        void ParseCastlingRightPart(std::string& splittedFen);
        void SplitFenPosition(std::vector<std::string>& fenParts);


    public:
        static Board *GetInstance();
        void CleanUp();
        Board(Board const&) = delete;
        Board& operator=(Board const&) = delete;

        BitBoards GetBitBoards() const;
        bool IsWhiteToMove() const;
        void PrintBoard() const;
        void SetBoard(const std::string& fenPosition);

    };
}
