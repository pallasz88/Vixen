#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stack>
#include "defs.h"
#include "slider.h"

namespace Vixen
{
    class Hash;

    void InitBitBoards(BitBoards &bitBoards);

    struct History
    {
        BitBoard enPassant;

        PositionKey hash;

        int castlingRights;

        int fiftyMoves;

        Move move;

        char movedPiece;

        char capturedPiece;

        explicit History(BitBoard enPassant,
                         int castlingRights,
                         int fiftyMoves,
                         Move move,
                         char moved,
                         char captured,
                         PositionKey hash) : enPassant(enPassant),
                                             hash(hash),
                                             castlingRights(castlingRights),
                                             fiftyMoves(fiftyMoves),
                                             move(move),
                                             movedPiece(moved),
                                             capturedPiece(captured)
        {}

    };

    class VIXEN_API Board
    {
    public:

        Board();

        BitBoards GetBitBoards() const
        { return bitBoards; }

        bool IsWhiteToMove() const
        { return whiteToMove; }

        BitBoard GetEnPassant() const
        { return enPassantBitBoard; }

        int GetCastlingRights() const
        { return castlingRights; }

        void PrintBoard() const;

        void SetBoard(const std::string &fenPosition);

        void RemovePiece(int position, char pieceType);

        void AddPiece(int position, char pieceType);

        bool MakeMove(Move move);

        void TakeBack();

        bool IsBoardConsistent() const;

        void SumUpBitBoards();

    private:

        std::stack<History> history;

        std::string fenPosition;

        char pieceList[SQUARE_NUMBER];

        BitBoards bitBoards;

        BitBoard enPassantBitBoard;

        int castlingRights;

        int historyMovesNum;

        int fiftyMoves;

        bool whiteToMove;

        std::unique_ptr<Hash> hashBoard;

        void ParseFenPiecePart(const std::string &splittedFen);

        void ParseSideToMovePart(const std::string &splittedFen);

        void ParseCastlingRightPart(const std::string &splittedFen);

        void SplitFenPosition(std::vector<std::string> &fenParts);

        void ClearHistory();

        void MakeCapture(int to, char capturedPieceLetter);

        void MakeDoublePawnPush(int enPassantSquare);

        void MoveCastlingWhiteRook(int from, int to);

        void MoveCastlingBlackRook(int from, int to);

        void UpdateCastlingRights(int from, int to);

        friend class MoveGenerator;
    };
}