#include "slider.h"

namespace Vixen
{
    BitBoard BishopAttacks[BISHOP_ATTACK_TABLE_SIZE];

    BitBoard RookAttacks[ROOK_ATTACK_TABLE_SIZE];

    Magic BishopTable[SQUARE_NUMBER];

    Magic RookTable[SQUARE_NUMBER];

    void InitMagics()
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
    void InitSlidingAttack(int square, SliderDirections directions, Magic *table)
    {
        BitBoard edges = ((FILEA | FILEH) & ~(FILEH << square % 8)) |
                         ((RANK1 | RANK8) & ~(RANK1 << 8 * (square / 8)));
        auto occupied = EMPTY_BOARD;

        table[square].magic = slider == Slider::BISHOP ? BishopMagic[square] : RookMagic[square];
        table[square].mask = SlidingAttack(square, directions, occupied) & ~edges;
        table[square].shift = SQUARE_NUMBER - PopCount(table[square].mask);

        if (square != MAX_SQUARE_INDEX)
            table[square + 1].attacks = table[square].attacks + (1 << PopCount(table[square].mask));

        do
        {
            int index = GetIndex(occupied, table[square]);
            table[square].attacks[index] = SlidingAttack(square, directions, occupied);
            occupied = (occupied - table[square].mask) & table[square].mask;
        } while (occupied);
    }

    BitBoard SlidingAttack(int square, SliderDirections directions, BitBoard occupied)
    {
        auto attacks = EMPTY_BOARD;
        for (const auto &direction : directions)
        {
            for (int file = square / 8 + direction[0], rank = square % 8 + direction[1];
                 IsValidCoordinate(file, rank);
                 GetNextCoordinate(file, rank, direction))
            {
                SetBit(attacks, 8 * file + rank);
                if (IsBitSet(occupied, 8 * file + rank))
                    break;
            }
        }

        return attacks;
    }
}