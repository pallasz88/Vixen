#pragma once

#include "defs.h"
#include "random.h"

namespace Vixen
{

class Board;

/**
 * Creates unique hash values from board positions.
 */
class VIXEN_API Hash
{
  public:
    /**
     * Sets hash value. In case undoing move it is faster
     * to recover hash from stack than recalculate it.
     * @param hash
     */
    constexpr void SetHash(PositionKey hash)
    {
        positionKey = hash;
    }

    /**
     * Gets hash value.
     */
    [[nodiscard]] constexpr PositionKey GetHash() const
    {
        return positionKey;
    }

    /**
     * For differentiating boards where enpassant is available or not.
     * @param enPassant
     */
    constexpr void HashEnPassant(BitBoard enPassant)
    {
        positionKey ^= zobristKeys.pieceHashKeys[TrailingZeroCount(enPassant)][enPassantKey];
    }

    /**
     * For differentiating boards where castling is available or not.
     * @param castlingRights
     */
    constexpr void HashCastling(unsigned castlingRights) noexcept
    {
        positionKey ^= zobristKeys.castleHashKeys[castlingRights];
    }

    /**
     * For hashing piece positions
     * @param square
     * @param pieceKey
     */
    constexpr void HashPiece(unsigned square, unsigned char pieceKey)
    {
        positionKey ^= zobristKeys.pieceHashKeys[square][static_cast<unsigned>(GetPieceIndex(pieceKey))];
    }

    /**
     * For differentiating boards where white's or black's turn.
     */
    constexpr void HashSide()
    {
        positionKey ^= sideKey;
    }

    static constexpr void InitZobristKeys()
    {
        unsigned i = 0;
        for (unsigned square = H1; square <= Constants::MAX_SQUARE_INDEX; ++square)
        {
            zobristKeys.pieceHashKeys[square][enPassantKey] = PRNG::GenerateRandom(++i);
            for (const auto &pieceKey : Constants::pieceKeys)
                zobristKeys.pieceHashKeys[square][static_cast<unsigned>(GetPieceIndex(pieceKey))] =
                    PRNG::GenerateRandom(++i);
        }

        for (auto &castleHashKey : zobristKeys.castleHashKeys)
            castleHashKey = PRNG::GenerateRandom(++i);

        zobristKeys.sideHashKey = sideKey;
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

    static constexpr PositionKey sideKey = PRNG::GenerateRandom(70);

    static Keys zobristKeys;

    static constexpr int enPassantKey = 12;
};
} // namespace Vixen
