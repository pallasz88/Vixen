#include <iostream>
#include "slider.h"

namespace Vixen
{
    BitBoard BishopAttacks[BishopAttackTableSize];

    BitBoard RookAttacks[RookAttackTableSize];

    SliderAttacks::SliderAttacks()
    {
        InitMagics();
    }

    void SliderAttacks::InitMagics()
    {
        RookTable[0].attacks = RookAttacks;
        BishopTable[0].attacks = BishopAttacks;
        for (int square = 0; square < SQUARE_NUMBER; ++square)
        {
            InitSlidingAttack<Slider::ROOK>(square, rookDirections, RookTable);
            InitSlidingAttack<Slider::BISHOP>(square, bishopDirections, BishopTable);
        }
    }

    template<Slider slider>
    void SliderAttacks::InitSlidingAttack(int square, SliderDirections directions, Magic *table)
    {
        BitBoard edges = FILEA | FILEH | RANK1 | RANK8;
        BitBoard occupied = EMPTYBOARD;

        table[square].magic = slider == Slider::BISHOP ? BishopMagic[square] : RookMagic[square];
        table[square].mask = SlidingAttack(square, directions, occupied) & ~edges;
        table[square].shift = SQUARE_NUMBER - PopCount(table[square].mask);

        if (square != SQUARE_NUMBER - 1)
            table[square + 1].attacks = table[square].attacks + (1 << PopCount(table[square].mask));

        do
        {
            int index = GetIndex(occupied, table[square]);
            table[square].attacks[index] = SlidingAttack(square, directions, occupied);
            occupied = (occupied - table[square].mask) & table[square].mask;
        } while (occupied);
    }

    BitBoard SliderAttacks::SlidingAttack(int square, SliderDirections directions, BitBoard occupied)
    {
        BitBoard result = EMPTYBOARD;
        for (const auto &direction : directions)
        {
            for (int file = square / 8 + direction[0], rank = square % 8 + direction[1];
                 IsValidCoordinate(file, rank);
                 GetNextCoordinate(file, rank, direction))
            {
                result |= 1ULL << (8 * file + rank);
                if (IsBitSet(occupied, BIT(8 * file + rank)))
                    break;
            }
        }

        return result;
    }

    void SliderAttacks::GetNextCoordinate(int &file, int &rank, const Direction &direction) const
    {
        file += direction[0], rank += direction[1];
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

    BitBoard SliderAttacks::GetQueenAttack(unsigned square, Vixen::BitBoard occupied)
    {
        return GetBishopAttack(square, occupied) | GetRookAttack(square, occupied);
    }
}