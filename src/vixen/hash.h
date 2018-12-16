#pragma once

#include "defs.h"

namespace Vixen
{
    class Board;

    class VIXEN_API Hash
    {
    public:

        explicit Hash(const Board &);

        BitBoard GetHash() const;

    private:

        BitBoard hash;

        std::map<int, std::map<char, BitBoard>> zobristHashKey;

        BitBoard GenerateBigRandom();

        void InitZobrist();

        void ComputeHash(const Board &);

        const std::array<char, 12> pieceKeys = {'P', 'N', 'B', 'R', 'Q', 'K',
                                                'p', 'n', 'b', 'r', 'q', 'k'};
    };
}
