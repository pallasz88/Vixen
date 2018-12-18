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

    void SliderAttacks::InitMagics(BitBoard occupied)
    {
        for (int square = 0; square < SQUARE_NUMBER; ++square)
        {
            SliderDirections directions = {{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}};
            RookTable[square].shift = SQUARE_NUMBER - RookBits[square];
            RookTable[square].magic = RookMagic[square];
            InitSlidingAttack(square, occupied, directions, RookTable, RookBits);
        }

        for (int square = 0; square < SQUARE_NUMBER; ++square)
        {
            SliderDirections directions = {{{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};
            BishopTable[square].shift = SQUARE_NUMBER - BishopBits[square];
            BishopTable[square].magic = BishopMagic[square];
            InitSlidingAttack(square, occupied, directions, BishopTable, BishopBits);
        }
    }

    void SliderAttacks::InitSlidingAttack(int square, BitBoard occupied, SliderDirections directions, Magic *table,
                                          const int shiftTable[])
    {
        BitBoard edges = FILEA | FILEH | RANK1 | RANK8;

        table[square].shift = static_cast<unsigned>(SQUARE_NUMBER - shiftTable[square]);
        table[0].attacks = BishopAttacks;
        table[square].mask = SlidingAttack(square, directions, occupied) & ~edges;
        if (square != SQUARE_NUMBER - 1)
            table[square + 1].attacks =
                    table[square].attacks + (1 << PopCount(table[square].mask));

        do
        {
            int index = GetIndex(occupied, table[square]);
            table[square].attacks[index] = SlidingAttack(square, directions, occupied);
            occupied = (occupied - table[square].mask) & table[square].mask;
        } while (occupied);
    }

    int SliderAttacks::GetIndex(BitBoard occupied, const Magic &table)
    {
        return ((occupied & table.mask) * table.magic) >> table.shift;
    }

    BitBoard SliderAttacks::GetBishopAttack(unsigned square, BitBoard occupied)
    {
        return BishopTable[square].attacks[GetIndex(occupied, BishopTable[square])];
    }

    BitBoard SliderAttacks::GetRookAttack(unsigned square, BitBoard occupied)
    {
        return RookTable[square].attacks[GetIndex(occupied, RookTable[square])];
    }

    BitBoard SliderAttacks::SlidingAttack(int square, SliderDirections directions, BitBoard occupied)
    {
        BitBoard result = 0ULL;
        for (const auto &direction : directions)
        {
            for (int coordinates[2] = {square / 8 + direction[0],
                                       square % 8 + direction[1]};
                 IsValidSquare(coordinates[0], coordinates[1]);
                 coordinates[0] += direction[0], coordinates[1] += direction[1])
            {
                result |= 1ULL << (8 * coordinates[0] + coordinates[1]);
                if (IsBitSet(occupied, BIT(8 * coordinates[0] + coordinates[1])))
                    break;
            }
        }

        return result;
    }

    SliderAttacks::SliderAttacks(BitBoard occupied)
    {
        InitMagics(occupied);
    }
}