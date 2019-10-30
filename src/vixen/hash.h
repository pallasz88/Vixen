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
        constexpr void HashEnPassant(BitBoard enPassant)
        { positionKey ^= pieceHashKeys[TrailingZeroCount(enPassant)][enPassantKey]; }

        /**
         * For differentiating boards where castling is available or not.
         * @param castlingRights
         */
        constexpr void HashCastling(int castlingRights) noexcept
        { positionKey ^= castleHashKeys[castlingRights]; }

        /**
         * For hashing piece positions
         * @param square
         * @param pieceKey
         */
        constexpr void HashPiece(int square, char pieceKey)
        { positionKey ^= pieceHashKeys[square][GetPieceIndex(pieceKey)]; }

        /**
         * For differentiating boards where white's or black's turn.
         */
        void HashSide()
        { positionKey ^= sideHashKey; }

        static void InitZobristKeys();

        void ComputePositionKey(const Board &board);

    private:

        PositionKey positionKey;

        static PieceHashKeys pieceHashKeys;

        static SideHashKey sideHashKey;

        static CastleHashKeys castleHashKeys;

        static constexpr int enPassantKey = 12;

        static BitBoard GenerateBigRandom();
    };
}
