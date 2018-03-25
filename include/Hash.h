#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <cstring>

namespace Vixen
{
    class DLL_EXPORT Hash
    {
    public:
        Hash();
        uint64_t GetHash() const;

        static Hash *GetInstance();
        void CleanUp();
        Hash(Hash const&) = delete;
        Hash& operator=(Hash const&) = delete;

    private:
        static Hash* instance;
        uint64_t hash;
        uint64_t zobristHashKey[64][12];
        void PrintZobrist();

        uint64_t GenerateBigRandom();
        void InitZobrist();
        void ComputeHash();
    };
}
