#include "defs.h"
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
        memset(&zobristHashKey, 0x00, sizeof(zobristHashKey));
        for (int i = H1; i < A8; ++i)
            for (int j = 0; j < 12; ++j)
                zobristHashKey[i][j] = GenerateBigRandom();
    }

    uint64_t Hash::GenerateBigRandom()
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
            if (~(bitBoards[' '] >> i) & 1)
            {
                if ((bitBoards['P'] >> i) & 1)
                    hash ^= zobristHashKey[i][0];

                else if ((bitBoards['p'] >> i) & 1)
                    hash ^= zobristHashKey[i][1];

                else if ((bitBoards['K'] >> i) & 1)
                    hash ^= zobristHashKey[i][2];

                else if ((bitBoards['k'] >> i) & 1)
                    hash ^= zobristHashKey[i][3];

                else if ((bitBoards['Q'] >> i) & 1)
                    hash ^= zobristHashKey[i][4];

                else if ((bitBoards['q'] >> i) & 1)
                    hash ^= zobristHashKey[i][5];

                else if ((bitBoards['R'] >> i) & 1)
                    hash ^= zobristHashKey[i][6];

                else if ((bitBoards['r'] >> i) & 1)
                    hash ^= zobristHashKey[i][7];

                else if ((bitBoards['N'] >> i) & 1)
                    hash ^= zobristHashKey[i][8];

                else if ((bitBoards['n'] >> i) & 1)
                    hash ^= zobristHashKey[i][9];

                else if ((bitBoards['B'] >> i) & 1)
                    hash ^= zobristHashKey[i][10];

                else if ((bitBoards['b'] >> i) & 1)
                    hash ^= zobristHashKey[i][11];

            }
        }
    }

    uint64_t Hash::GetHash() const
    {
        return hash;
    }

    void Hash::PrintZobrist()
    {
        for (int i = H1; i < A8; ++i)
            for (int j = 0; j < 12; ++j)
                std::cout << "(" << i << "," << j << "): " << zobristHashKey[i][j] << std::endl;
    }
}