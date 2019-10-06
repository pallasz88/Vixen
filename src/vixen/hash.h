#pragma once

#include "board.h"

namespace Vixen
{
    /**
     * Creates unique hash values from board positions.
     */
    class Hash
    {
    public:

        /**
         * Constructor
         * @param board
         */
        explicit Hash(const Board &board);

        /**
         * Returns position key.
         */
        [[nodiscard]] constexpr PositionKey GetHash() const
        { return positionKey; }

        /**
         * Sets hash value. In case undoing move it is faster
         * to recover hash from stack than recalculate it.
         * @param hash
         */
        constexpr void SetHash(BitBoard hash)
        { positionKey = hash; }

        /**
         * For differentiating boards where enpassant is available or not.
         * @param enPassant
         */
        void HashEnPassant(BitBoard enPassant);

        /**
         * For differentiating boards where castling is available or not.
         * @param board
         */
        constexpr void HashCastling(const Board &board);

        /**
         * For hashing piece positions
         * @param square
         * @param pieceKey
         */
        void HashPiece(int square, char pieceKey);

        /**
         * For differentiating boards where white's or black's turn.
         */
        constexpr void HashSide();

    private:

        PositionKey positionKey;

        PieceHashKeys pieceHashKeys;

        SideHashKey sideHashKey;

        CastleHashKeys castleHashKeys;

        static BitBoard GenerateBigRandom();

        void InitZobristKeys();

        void ComputePositionKey(const Board &board);

        static constexpr char enPassantKey = ' ';
    };

    inline constexpr void Hash::HashCastling(const Board &board)
    {
        positionKey ^= castleHashKeys.at(static_cast<uint8_t>(board.GetCastlingRights()));
    }

    inline constexpr void Hash::HashSide()
    {
        positionKey ^= sideHashKey;
    }

    inline void Hash::HashPiece(int square, char pieceKey)
    {
        positionKey ^= pieceHashKeys[square][pieceKey];
    }

    inline void Hash::HashEnPassant(BitBoard enPassant)
    {
        positionKey ^= pieceHashKeys[TrailingZeroCount(enPassant)][enPassantKey];
    }
}
