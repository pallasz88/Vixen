#include <iostream>
#include "slider.h"

namespace Vixen
{
    BitBoard BishopAttacks[BishopAttackTableSize];

    BitBoard RookAttacks[RookAttackTableSize];

    int RookBits[SQUARE_NUMBER] = {
            12, 11, 11, 11, 11, 11, 11, 12,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            12, 11, 11, 11, 11, 11, 11, 12
    };

    int BishopBits[SQUARE_NUMBER] = {
            6, 5, 5, 5, 5, 5, 5, 6,
            5, 5, 5, 5, 5, 5, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5,
            6, 5, 5, 5, 5, 5, 5, 6
    };

    void InitMagics()
    {
        for (int square = 0; square < SQUARE_NUMBER; ++square)
        {
            RookTable[square].shift = SQUARE_NUMBER - RookBits[square];
            RookTable[square].magic = RookMagic[square];
            RookTable[square].InitSlidingAttack(square);
        }

        for (int square = 0; square < SQUARE_NUMBER; ++square)
        {
            BishopTable[square].shift = SQUARE_NUMBER - BishopBits[square];
            BishopTable[square].magic = BishopMagic[square];
            BishopTable[square].InitSlidingAttack(square);
        }
    }

    template<Slider slider>
    void Magic<slider>::InitSlidingAttack(unsigned square)
    {
        memset(BishopAttacks, 0ULL, sizeof(BitBoard) * BishopAttackTableSize);
        memset(RookAttacks, 0ULL, sizeof(BitBoard) * RookAttackTableSize);
        attacks = (slider == Slider::BISHOP) ? BishopAttacks : RookAttacks;
        mask = SlidingAttack(square);
        attacks[square] = mask;
    }

    template<Slider slider>
    BitBoard Magic<slider>::GetIndex(BitBoard occupied)
    {
        return ((occupied & mask) * magic) >> shift;
    }

    template<>
    BitBoard Magic<Slider::BISHOP>::GetAttack(unsigned square, BitBoard occupied)
    {
        return BishopTable[square].attacks[BishopTable[square].GetIndex(occupied)];
    }

    template<>
    BitBoard Magic<Slider::ROOK>::GetAttack(unsigned square, BitBoard occupied)
    {
        return RookTable[square].attacks[RookTable[square].GetIndex(occupied)];
    }

    template<Slider slider>
    BitBoard Magic<slider>::SlidingAttack(int square)
    {
        BitBoard result = 0ULL;
        const int directions[4][2] = {{-1, -1},
                                      {-1, 1},
                                      {1,  -1},
                                      {1,  1}};
        const int directions[4][2] = {{-1, 0},
                                      {0,  -1},
                                      {1,  0},
                                      {0,  1}};
        for (const auto &direction : directions)
        {

            for (int coordinates[2] = {square / 8 + direction[0],
                                       square % 8 + direction[1]};
                 IsValidSquare(coordinates[0], coordinates[1]);
                 coordinates[0] += direction[0], coordinates[1] += direction[1])
                result |= 1ULL << (8 * coordinates[0] + coordinates[1]);
        }

        return result;
    }

    Magic<Slider::BISHOP> BishopTable[SQUARE_NUMBER];

    Magic<Slider::ROOK> RookTable[SQUARE_NUMBER];
}