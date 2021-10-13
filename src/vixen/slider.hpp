#ifndef VIXEN_SLIDER_HPP_INCLUDED
#define VIXEN_SLIDER_HPP_INCLUDED

#include <cassert>

#include "defs.hpp"

namespace vixen::slider_utils
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

extern std::array<Magic, Constants::SQUARE_NUMBER> BishopTable;

extern std::array<Magic, Constants::SQUARE_NUMBER> RookTable;

constexpr unsigned GetIndex(BitBoard occupied, const Magic &table) noexcept;

inline BitBoard GetBishopAttack(unsigned int square, BitBoard occupied) noexcept;

inline BitBoard GetRookAttack(unsigned int square, BitBoard occupied) noexcept;

constexpr void GetNextCoordinate(int &file, int &rank, const Direction &direction) noexcept;

constexpr BitBoard SlidingAttack(unsigned int square, SliderDirections directions, BitBoard occupied) noexcept;

template <Slider slider>
constexpr void InitSlidingAttack(unsigned int square, SliderDirections directions,
                                 std::array<Magic, Constants::SQUARE_NUMBER> &table) noexcept;

inline void InitMagics() noexcept;

#include "slider.inl"

} // namespace Vixen::slider_utils

#endif // VIXEN_SLIDER_HPP_INCLUDED
