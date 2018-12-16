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
        for (unsigned square = 0; square < SQUARE_NUMBER; ++square)
        {
            RookTable[square].shift = static_cast<unsigned>(SQUARE_NUMBER - RookBits[square]);
            RookTable[square].magic = RookMagic[square];
            RookTable[square].InitSlidingAttack(square);
        }

        for (unsigned square = 0; square < SQUARE_NUMBER; ++square)
        {
            BishopTable[square].shift = static_cast<unsigned>(SQUARE_NUMBER - BishopBits[square]);
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
    BitBoard Magic<slider>::SlidingAttack(unsigned square)
    {
        BitBoard result = 0ULL;
        const int bishopDirections[4][2] =
                {{-1, -1},
                 {-1, 1},
                 {1,  -1},
                 {1,  1}};
        for (auto bishopDirection : bishopDirections)
        {
            for (int newSquare = GetNextSquare(square, bishopDirection);
                 IsValidSquare(newSquare % 8, newSquare / 8);
                 newSquare = GetNextSquare(newSquare, bishopDirection))
                result |= 1ULL << newSquare;

        }

        return result;
    }

    Magic<Slider::BISHOP> BishopTable[SQUARE_NUMBER];

    Magic<Slider::ROOK> RookTable[SQUARE_NUMBER];
}