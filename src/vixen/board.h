#pragma once

#include <memory>
#include <vector>
#include <string>
#include "defs.h"

namespace Vixen
{
    class Hash;

    class MoveGenerator;

    void InitBitBoards(BitBoards &bitBoards);

    class VIXEN_API Board
    {
    public:

        Board();

        inline BitBoards GetBitBoards() const
        { return bitBoards; }

        inline MoveGenerator &GetMoveGenerator() const
        { return *generator; }

        inline bool IsWhiteToMove() const
        { return whiteToMove; }

        inline std::string GetEnPassant() const
        { return enpassant; }

        void PrintBoard() const;

        void SetBoard(const std::string &fenPosition);

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

        void ParseFenPiecePart(const std::string &splittedFen);

        void ParseSideToMovePart(const std::string &splittedFen);

        void ParseCastlingRightPart(const std::string &splittedFen);

        void SplitFenPosition(std::vector<std::string> &fenParts);

    };
}