#include "hash.h"
#include "board.h"

#include <iostream>
#include <random>

namespace Vixen
{
    Hash::Hash(const Board &board)
    {
        InitZobrist();
        ComputeHash(board);
#ifdef DEBUG
        std::cout << hash << std::endl;
#endif
    }

    void Hash::InitZobrist()
    {
        hash = 0;
        for (int square = H1; A8 >= square; ++square)
            for (auto &pieceKey : pieceKeys)
                zobristHashKey[square][pieceKey] = GenerateBigRandom();
    }

    BitBoard Hash::GenerateBigRandom()
    {
        static std::default_random_engine generator(std::random_device{}());
        static std::uniform_int_distribution<uint64_t> distribution(0, UINTMAX_MAX);
        return distribution(generator);
    }

    void Hash::ComputeHash(const Board &board)
    {
        BitBoards bitBoards = board.GetBitBoards();
        for (int square = MAX_SHIFT_NUM; square >= 0; --square)
        {
            for (const auto &pieceKey : pieceKeys)
            {
                if (~(bitBoards[' '] >> square) & 1)
                    if ((bitBoards[pieceKey] >> square) & 1)
                        hash ^= zobristHashKey[square][pieceKey];
            }
        }
    }

    BitBoard Hash::GetHash() const
    {
        return hash;
    }
}