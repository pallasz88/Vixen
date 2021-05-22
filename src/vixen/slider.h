#pragma once

#include <cassert>

#include "defs.h"

namespace Vixen::SliderUtils
{
struct Magic
{
    /**
     * Pointer to attack_table for each particular square
     */
    BitBoard *attacks;

    /**
     * Mask relevant squares of both lines (no outer squares)
     */
    BitBoard mask;

    /**
     * Magic 64-bit factor
     */
    BitBoard magic;

    /**
     * Shift right
     */
    unsigned shift;
};

extern BitBoard BishopAttacks[Constants::BISHOP_ATTACK_TABLE_SIZE];

extern BitBoard RookAttacks[Constants::ROOK_ATTACK_TABLE_SIZE];

extern Magic BishopTable[Constants::SQUARE_NUMBER];

extern Magic RookTable[Constants::SQUARE_NUMBER];

constexpr unsigned GetIndex(BitBoard occupied, const Magic &table) noexcept
{
    assert(table.shift < Constants::SQUARE_NUMBER);
    return static_cast<unsigned>(((occupied & table.mask) * table.magic) >> table.shift);
}

inline BitBoard GetBishopAttack(unsigned int square, BitBoard occupied) noexcept
{
    return SliderUtils::BishopTable[square].attacks[GetIndex(occupied, SliderUtils::BishopTable[square])];
}

inline BitBoard GetRookAttack(unsigned int square, BitBoard occupied) noexcept
{
    return SliderUtils::RookTable[square].attacks[GetIndex(occupied, SliderUtils::RookTable[square])];
}

constexpr void GetNextCoordinate(int &file, int &rank, const Direction &direction) noexcept
{
    file += direction[0], rank += direction[1];
}

void InitMagics() noexcept;

template <Slider slider>
void InitSlidingAttack(unsigned int square, SliderDirections directions, Magic *table) noexcept;

constexpr BitBoard SlidingAttack(unsigned int square, SliderDirections directions, BitBoard occupied) noexcept;
} // namespace Vixen::SliderUtils
