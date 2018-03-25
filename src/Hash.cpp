#include "Hash.h"
#include "Board.h"
#include "defs.h"

#include <iostream>
#include <random>

using namespace Vixen;

Hash* Hash::instance = NULL;

Hash::Hash()
{
    InitZobrist();
    ComputeHash();
}

Hash* Hash::GetInstance()
{
    if(!instance)
        instance = new Hash();

    return instance;
}

void Hash::CleanUp()
{
    if(instance)
        delete instance;
}

void Hash::InitZobrist()
{
    hash = 0;
    memset( &zobristHashKey, 0x00, sizeof( zobristHashKey ) );
    for (int i = H1; i < A8; ++i)
        for (int j = 0; j < 12; ++j)
            zobristHashKey[i][j] = GenerateBigRandom();
}

uint64_t Hash::GenerateBigRandom()
{
    static default_random_engine generator(random_device{}());
    static uniform_int_distribution<uint64_t> distribution(0,UINTMAX_MAX);
    return distribution(generator);
}

void Hash::ComputeHash()
{
    BitBoards bitBoards = Board::GetInstance()->GetBitBoards();
    for (int i = MAX_SHIFT_NUM; i >= 0; i--)
    {
        if ((bitBoards.occupied >> i)&1)
        {
            if ((bitBoards.P >> i)&1)
                hash ^= zobristHashKey[i][0];

            else if((bitBoards.p >> i)&1)
                hash ^= zobristHashKey[i][1];

            else if((bitBoards.K >> i)&1)
                hash ^= zobristHashKey[i][2];

            else if((bitBoards.k >> i)&1)
                hash ^= zobristHashKey[i][3];

            else if((bitBoards.Q >> i)&1)
                hash ^= zobristHashKey[i][4];

            else if((bitBoards.q >> i)&1)
                hash ^= zobristHashKey[i][5];

            else if((bitBoards.R >> i)&1)
                hash ^= zobristHashKey[i][6];

            else if((bitBoards.r >> i)&1)
                hash ^= zobristHashKey[i][7];

            else if((bitBoards.N >> i)&1)
                hash ^= zobristHashKey[i][8];

            else if((bitBoards.n >> i)&1)
                hash ^= zobristHashKey[i][9];

            else if((bitBoards.B >> i)&1)
                hash ^= zobristHashKey[i][10];

            else if((bitBoards.b >> i)&1)
                hash ^= zobristHashKey[i][11];

        }
    }
}

uint64_t Hash::GetHash() const
{
    cout << hash << endl;
    return hash;
}

void Hash::PrintZobrist()
{
    for (int i = H1; i < A8; ++i)
        for (int j = 0; j < 12; ++j)
            cout << "(" << i << "," << j << "): " << zobristHashKey[i][j] << endl;
}
