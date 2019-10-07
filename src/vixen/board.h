#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stack>
#include <memory>
#include "defs.h"
#include "slider.h"

namespace Vixen
{
    class Hash;

    class MoveGenerator;

    /**
     * Used for recovering previous positions to start position.
     */
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

    /**
     * Holds the representation of chess board.
     */
    class VIXEN_API Board
    {
    public:

        /**
         * Constructor
         */
        Board();

        /**
         * Returns piece bitboard from bitBoards
         * @param piece
         * @return
         */
        [[nodiscard]] constexpr BitBoard GetBitBoard(char piece) const
        {
            return bitBoards[GetPieceIndex(piece)];
        }

        /**
         * Returns if white is on move from given position.
         * @return whiteToMove
         */
        [[nodiscard]] constexpr bool IsWhiteToMove() const
        { return whiteToMove; }

        /**
         * Returns a bitboard for enpassant from given position.
         * @return enPassantBitBoard
         */
        [[nodiscard]] constexpr BitBoard GetEnPassant() const
        { return enPassantBitBoard; }

        /**
         * Returns castling rights from given position.
         * @return castlingRights
         */
        [[nodiscard]] constexpr int GetCastlingRights() const
        { return castlingRights; }

        /**
         * Prints the board to console.
         */
        void PrintBoard() const;

        /**
         * Sets board from FEN position.
         *  FEN position: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
         * @param fenPosition
         */
        void SetBoard(const std::string &fenPosition);

        /**
         * Removes given piece type from given position.
         * @param position
         * @param pieceType
         */
        void RemovePiece(int position, char pieceType);

        /**
         * Adds given piece type to given position.
         * @param position
         * @param pieceType
         */
        void AddPiece(int position, char pieceType);

        /**
         * Tries to move a pseudo-legal move.
         * If after the move turns out king is in check, function rolls back to previous state.
         * Warning: Make sure, that the move is at least pseudo-legal, the function does not check it.
         * @param move
         * @return
         */
        bool MakeMove(Move move);

        /**
         * Rolls back to previous state of the board.
         */
        void TakeBack();

        /**
         * Returns a generator of pseudo-legal moves from given position.
         * @return pseudo-legal moves
         */
        [[nodiscard]] MoveGenerator CreateGenerator() const;

    private:

        std::stack<History> history;

        std::string fenPosition;

        std::array<char, SQUARE_NUMBER> pieceList;

        BitBoards bitBoards;

        BitBoard enPassantBitBoard;

        int castlingRights;

        int historyMovesNum;

        int fiftyMoves;

        bool whiteToMove;

        std::unique_ptr<Hash> hashBoard;

        void SumUpBitBoards();

        void AddHashBoard();

        void ParseFenPiecePart(const std::string &splittedFen);

        void ParseSideToMovePart(const std::string &splittedFen);

        void ParseCastlingRightPart(const std::string &splittedFen);

        [[nodiscard]] std::vector<std::string> SplitFenPosition() const;

        void ClearHistory();

        void MakeCapture(int to, char capturedPieceLetter);

        void MakeDoublePawnPush(int enPassantSquare);

        void MoveCastlingWhiteRook(int from, int to);

        void MoveCastlingBlackRook(int from, int to);

        void UpdateCastlingRights(int from, int to);
    };
}