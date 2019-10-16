#pragma once

#include "defs.h"

namespace Vixen
{

    class Board;

    /**
     * Creates unique hash values from board positions.
     */
    class Hash
    {
    public:

        /**
         * Sets hash value. In case undoing move it is faster
         * to recover hash from stack than recalculate it.
         * @param hash
         */
        constexpr void SetHash(PositionKey hash)
        { positionKey = hash; }

        /**
         * Gets hash value.
         */
        [[nodiscard]] constexpr PositionKey GetHash() const
        { return positionKey; }

        /**
         * For differentiating boards where enpassant is available or not.
         * @param enPassant
         */
        void HashEnPassant(BitBoard enPassant);

        /**
         * For differentiating boards where castling is available or not.
         * @param board
         */
        void HashCastling(const Board &board);

        /**
         * For hashing piece positions
         * @param square
         * @param pieceKey
         */
        void HashPiece(int square, char pieceKey);

        /**
         * For differentiating boards where white's or black's turn.
         */
        void HashSide();

        static void InitZobristKeys();

        void ComputePositionKey(const Board &board);

    private:

        PositionKey positionKey;

        static PieceHashKeys pieceHashKeys;

        static SideHashKey sideHashKey;

        static CastleHashKeys castleHashKeys;

        static BitBoard GenerateBigRandom();

        static constexpr char enPassantKey = ' ';
    };
}
