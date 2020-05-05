#include "slider.h"

namespace Vixen::SliderUtils
{
    BitBoard BishopAttacks[Constants::BISHOP_ATTACK_TABLE_SIZE];

    BitBoard RookAttacks[Constants::ROOK_ATTACK_TABLE_SIZE];

    Magic BishopTable[Constants::SQUARE_NUMBER];

    Magic RookTable[Constants::SQUARE_NUMBER];

    void InitMagics()
    {
        RookTable[0].attacks   = RookAttacks;
        BishopTable[0].attacks = BishopAttacks;
        for (unsigned square   = 0; square < Constants::SQUARE_NUMBER; ++square)
        {
            InitSlidingAttack<Slider::ROOK>(square, Constants::rookDirections, RookTable);
            InitSlidingAttack<Slider::BISHOP>(square, Constants::bishopDirections, BishopTable);
        }
    }

    template<Slider slider>
    void InitSlidingAttack(unsigned int square, SliderDirections directions, Magic *table)
    {
        BitBoard edges    = ((FILEA | FILEH) & ~(FILEH << square % 8)) |
                            ((RANK1 | RANK8) & ~(RANK1 << 8 * (square / 8)));
        auto     occupied = Constants::EMPTY_BOARD;

        table[square].magic = slider == Slider::BISHOP ? Constants::BishopMagic[square] : Constants::RookMagic[square];
        table[square].mask  = SlidingAttack(square, directions, occupied) & ~edges;
        table[square].shift = Constants::SQUARE_NUMBER - PopCount(table[square].mask);

        if (square != Constants::MAX_SQUARE_INDEX)
            table[square + 1].attacks = table[square].attacks + (1U << PopCount(table[square].mask));

        do
        {
            unsigned index = GetIndex(occupied, table[square]);
            table[square].attacks[index] = SlidingAttack(square, directions, occupied);
            occupied = (occupied - table[square].mask) & table[square].mask;
        } while (occupied);
    }

    BitBoard SlidingAttack(unsigned int square, SliderDirections directions, BitBoard occupied)
    {
        auto            attacks = Constants::EMPTY_BOARD;
        for (const auto &direction : directions)
        {
            for (int file = static_cast<int>(square / 8) + direction[0],
                     rank = static_cast<int>(square % 8) + direction[1];
                 IsValidCoordinate(file, rank);
                 GetNextCoordinate(file, rank, direction))
            {
                SetBit(attacks, static_cast<unsigned>(8 * file + rank));
                if (IsBitSet(occupied, static_cast<unsigned>(8 * file + rank)))
                    break;
            }
        }

        return attacks;
    }
}