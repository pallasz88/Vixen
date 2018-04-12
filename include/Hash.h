#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <cstring>

namespace Vixen
{
    class Board;

    class DLL_EXPORT Hash
    {
    public:
        uint64_t GetHash() const;

        void CleanUp();
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
