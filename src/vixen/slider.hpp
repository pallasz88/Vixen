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

    [[nodiscard]] constexpr unsigned GetIndex(BitBoard occupied) const noexcept
    {
        assert(shift < Constants::SQUARE_NUMBER);
        return static_cast<unsigned>(((occupied & mask) * magic) >> shift);
    }

    [[nodiscard]] constexpr BitBoard GetAttack(BitBoard occupied) const noexcept
    {
        return attacks[GetIndex(occupied)];
    }
};

extern const std::array<Magic, Constants::SQUARE_NUMBER> BishopTable;

extern const std::array<Magic, Constants::SQUARE_NUMBER> RookTable;

inline BitBoard GetBishopAttack(unsigned int square, BitBoard occupied) noexcept;

inline BitBoard GetRookAttack(unsigned int square, BitBoard occupied) noexcept;

constexpr void GetNextCoordinate(int &file, int &rank, const Direction &direction) noexcept;

constexpr BitBoard SlidingAttack(unsigned int square, SliderDirections directions, BitBoard occupied) noexcept;

template <Slider slider>
constexpr void InitSlidingAttack(unsigned int square, SliderDirections directions,
                                 std::array<Magic, Constants::SQUARE_NUMBER> &table) noexcept;

template <class SliderInfo>
constexpr auto initMagics = []() {
    static std::array<BitBoard, SliderInfo::attackTableSize> attacks{};
    std::array<Magic, Constants::SQUARE_NUMBER> table{};
    table[0].attacks = &attacks[0];
    for (unsigned square = 0; square < Constants::SQUARE_NUMBER; ++square)
        InitSlidingAttack<SliderInfo::slider>(square, SliderInfo::directions, table);

    return table;
};

struct RookSliderInfo
{
    static const size_t attackTableSize = Constants::ROOK_ATTACK_TABLE_SIZE;

    static const Slider slider = Slider::ROOK;

    static constexpr SliderDirections directions = Constants::rookDirections;
};

struct BishopSliderInfo
{
    static const size_t attackTableSize = Constants::BISHOP_ATTACK_TABLE_SIZE;

    static const Slider slider = Slider::BISHOP;

    static constexpr SliderDirections directions = Constants::bishopDirections;
};
#include "slider.inl"

} // namespace vixen::slider_utils

#endif // VIXEN_SLIDER_HPP_INCLUDED
