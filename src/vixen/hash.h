#pragma once

#include "defs.h"
#include "random.h"

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
        { positionKey ^= zobristKeys.pieceHashKeys[TrailingZeroCount(enPassant)][enPassantKey]; }

        /**
         * For differentiating boards where castling is available or not.
         * @param castlingRights
         */
        constexpr void HashCastling(int castlingRights) noexcept
        { positionKey ^= zobristKeys.castleHashKeys[castlingRights]; }

        /**
         * For hashing piece positions
         * @param square
         * @param pieceKey
         */
        constexpr void HashPiece(int square, char pieceKey)
        { positionKey ^= zobristKeys.pieceHashKeys[square][GetPieceIndex(pieceKey)]; }

        /**
         * For differentiating boards where white's or black's turn.
         */
        void HashSide()
        { positionKey ^= zobristKeys.sideHashKey; }

        static constexpr void InitZobristKeys()
        {
            int i = -1;
            for (int square = H1; square <= MAX_SQUARE_INDEX; ++square)
            {
                zobristKeys.pieceHashKeys[square][enPassantKey] = PRNG::GenerateRandom(++i);
                for (const auto &pieceKey : pieceKeys)
                    zobristKeys.pieceHashKeys[square][GetPieceIndex(pieceKey)] = PRNG::GenerateRandom(++i);
            }

            zobristKeys.sideHashKey = PRNG::GenerateRandom(++i);

            for (auto &castleHashKey : zobristKeys.castleHashKeys)
                castleHashKey = PRNG::GenerateRandom(++i);

        }

        void ComputePositionKey(const Board &board);

        struct Keys
        {
            PieceHashKeys pieceHashKeys;
            SideHashKey sideHashKey;
            CastleHashKeys castleHashKeys;
        };

    private:

        PositionKey positionKey;

        static Keys zobristKeys;

        static constexpr int enPassantKey = 12;

    };
}
