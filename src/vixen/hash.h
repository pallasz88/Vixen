#pragma once

#include <cstdint>
#include <cstring>
#include "defs.h"

namespace Vixen
{
    class Board;

    class VIXEN_API Hash
    {
    public:
        uint64_t GetHash() const;

        Hash(const Board&);
        ~Hash() = default;
        Hash(Hash const&) = delete;
        Hash(Hash&&) = delete;

    private:
        uint64_t hash;
        uint64_t zobristHashKey[64][12];
        void PrintZobrist();
        uint64_t GenerateBigRandom();
        void InitZobrist();
        void ComputeHash(const Board&);
    };
}
