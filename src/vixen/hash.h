#pragma once

#include "defs.h"

namespace Vixen
{
    class Board;

    class Hash
    {
    public:

        explicit Hash(const Board &);

        BitBoard GetHash() const;

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
