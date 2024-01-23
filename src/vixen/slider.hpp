#ifndef SRC_VIXEN_SLIDER_HPP_
#define SRC_VIXEN_SLIDER_HPP_

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

constexpr void GetNextCoordinate(int &file, int &rank, const DirectionVector &direction) noexcept;

constexpr BitBoard SlidingAttack(unsigned int square, const SliderDirections &directions, BitBoard occupied) noexcept;

/**
 * Initializes the magic numbers and attack tables for a sliding piece (bishop or rook) at a given square.
 *
 * @tparam slider   The type of sliding piece. Must be either `Slider::BISHOP` or `Slider::ROOK`.
 * @param square    The square on the chessboard for which to initialize the magic numbers and attack tables.
 * @param directions The directions in which the sliding piece can move.
 * @param table     The table of magic numbers and attack tables to be initialized.
 */
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

#endif // SRC_VIXEN_SLIDER_HPP_
