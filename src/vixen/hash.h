#pragma once

#include "defs.h"

namespace Vixen
{
    class Board;

    class Hash
    {
    public:

        explicit Hash(const Board &);

        inline PositionKey GetHash() const
        { return positionKey; }

        inline void SetHash(BitBoard hash)
        { positionKey = hash; }

        void HashEnPassant(BitBoard enPassant);

        void HashCastling(const Board &board);

        void HashPiece(int square, const char &pieceKey);

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
}
