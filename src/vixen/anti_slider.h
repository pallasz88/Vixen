#ifndef ANTI_SLIDER_HPP_INCLUDED
#define ANTI_SLIDER_HPP_INCLUDED

#include "defs.h"

namespace Vixen::AntSliderUtils
{
extern const std::array<std::array<BitBoard, Constants::SQUARE_NUMBER>, Constants::COLOR_NUMBER> pawnAttack;

extern const std::array<BitBoard, Constants::SQUARE_NUMBER> knightAttack;

extern const std::array<BitBoard, Constants::SQUARE_NUMBER> kingAttack;

template <AntiSliderDirections offset>
[[nodiscard]] constexpr auto Init8DirectionAttack() noexcept -> std::array<BitBoard, Constants::SQUARE_NUMBER>;

constexpr auto InitPawnAttack() noexcept
    -> std::array<std::array<BitBoard, Constants::SQUARE_NUMBER>, Constants::COLOR_NUMBER>;
} // namespace Vixen::AntSliderUtils

#include "anti_slider.inl"
#endif
