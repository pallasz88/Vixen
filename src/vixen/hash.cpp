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
        for (int squareKey = H1; squareKey < A8; ++squareKey)
            for (auto &pieceKey : pieceKeys)
                zobristHashKey[squareKey][pieceKey] = GenerateBigRandom();
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
        for (int i = MAX_SHIFT_NUM; i >= 0; i--)
        {
            for (const auto &pieceKey : pieceKeys)
            {
                if (~(bitBoards[' '] >> i) & 1)
                    if ((bitBoards[pieceKey] >> i) & 1)
                        hash ^= zobristHashKey[i][pieceKey];
            }
        }
    }

    BitBoard Hash::GetHash() const
    {
        return hash;
    }
}