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
            RookTable[square].shift = SQUARE_NUMBER - RookBits[square];
            RookTable[square].magic = RookMagic[square];
            InitSlidingAttack<Slider::ROOK>(square, occupied);
        }

        for (int square = 0; square < SQUARE_NUMBER; ++square)
        {
            BishopTable[square].shift = SQUARE_NUMBER - BishopBits[square];
            BishopTable[square].magic = BishopMagic[square];
            InitSlidingAttack<Slider::BISHOP>(square, occupied);
        }
    }

    template<Slider slider>
    void SliderAttacks::InitSlidingAttack(int square, BitBoard occupied)
    {
        BitBoard edges = FILEA | FILEH | RANK1 | RANK8;

        if (SliderDirections directions = {{{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}}; slider == Slider::BISHOP)
        {
            BishopTable[square].shift = static_cast<unsigned>(SQUARE_NUMBER - RookBits[square]);
            BishopTable[0].attacks = BishopAttacks;
            BishopTable[square].mask = SlidingAttack(square, directions, occupied) & ~edges;
            if (square != SQUARE_NUMBER - 1)
                BishopTable[square + 1].attacks =
                        BishopTable[square].attacks + (1 << PopCount(BishopTable[square].mask));

            do
            {
                int index = GetIndex(occupied, BishopTable[square]);
                BishopTable[square].attacks[index] = SlidingAttack(square, directions, occupied);
                occupied = (occupied - BishopTable[square].mask) & BishopTable[square].mask;
            } while (occupied);
        }

        if (SliderDirections directions = {{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}}; slider == Slider::ROOK)
        {
            RookTable[square].shift = static_cast<unsigned>(SQUARE_NUMBER - RookBits[square]);
            RookTable[0].attacks = RookAttacks;
            RookTable[square].mask = SlidingAttack(square, directions, occupied) & ~edges;

            if (square != SQUARE_NUMBER - 1)
                RookTable[square + 1].attacks =
                        RookTable[square].attacks + (1 << PopCount(RookTable[square].mask));

            do
            {
                int index = GetIndex(occupied, RookTable[square]);
                RookTable[square].attacks[index] = SlidingAttack(square, directions, occupied);
                occupied = (occupied - RookTable[square].mask) & RookTable[square].mask;
            } while (occupied);
        }
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