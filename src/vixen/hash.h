#pragma once

#include "board.h"

namespace Vixen
{
    class Hash
    {
    public:

        explicit Hash(const Board &);

        PositionKey GetHash() const
        { return positionKey; }

        void SetHash(BitBoard hash)
        { positionKey = hash; }

        void HashEnPassant(BitBoard enPassant);

        void HashCastling(const Board &board);

        void HashPiece(int square, char pieceKey);

        void HashSide();

    private:

        PositionKey positionKey;

        PieceHashKeys pieceHashKeys;

        SideHashKey sideHashKey;

        CastleHashKeys castleHashKeys;

        BitBoard GenerateBigRandom();

        void InitZobristKeys();

        void ComputePositionKey(const Board &board);

        const std::array<char, 12> pieceKeys = {'P', 'N', 'B', 'R', 'Q', 'K',
                                                'p', 'n', 'b', 'r', 'q', 'k'};

        char enPassantKey = ' ';
    };

    inline void Hash::HashCastling(const Board &board)
    {
        positionKey ^= castleHashKeys.at(static_cast<uint8_t>(board.GetCastlingRights()));
    }

    inline void Hash::HashSide()
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
