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

        int castlingRights;

        int fiftyMoves;

        Move move;

        char movedPiece;

        char capturedPiece;

        PositionKey hash;

        explicit History(BitBoard enPassant, int castlingRights,
                         int fiftyMoves, Move move, char moved, char captured,
                         PositionKey hash) : enPassant(enPassant),
                                             castlingRights(castlingRights),
                                             fiftyMoves(fiftyMoves),
                                             move(move),
                                             movedPiece(moved),
                                             capturedPiece(captured),
                                             hash(hash)
        {}

    };

    class VIXEN_API Board
    {
    public:

        Board();

        inline BitBoards GetBitBoards() const
        { return bitBoards; }

        inline bool IsWhiteToMove() const
        { return whiteToMove; }

        inline BitBoard GetEnPassant() const
        { return enPassant; }

        inline int GetCastlingRights() const
        { return castlingRights; }

        inline SliderAttacks GetSlider() const
        { return sliders; }

        void PrintBoard() const;

        void SetBoard(const std::string &fenPosition);

        void RemovePiece(int position, char pieceType);

        void AddPiece(int position, char pieceType);

        bool MakeMove(Move move);

        void TakeBack();

    private:

        SliderAttacks sliders;

        std::unique_ptr<Hash> hashBoard;

        std::string fenPosition;

        BitBoards bitBoards;

        char pieceList[SQUARE_NUMBER];

        BitBoard enPassant;

        int castlingRights;

        int historyMovesNum;

        int fiftyMoves;

        bool whiteToMove;

        std::stack<History> history;

        void ParseFenPiecePart(const std::string &splittedFen);

        void ParseSideToMovePart(const std::string &splittedFen);

        void ParseCastlingRightPart(const std::string &splittedFen);

        void SplitFenPosition(std::vector<std::string> &fenParts);

        inline auto GetPieceBoard(int position)
        { return pieceList[position]; }

        void ClearHistory();
    };
}