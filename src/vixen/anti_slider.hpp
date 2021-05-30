#ifndef VIXEN_ANTI_SLIDER_HPP_INCLUDED
#define VIXEN_ANTI_SLIDER_HPP_INCLUDED

#include "defs.hpp"

namespace Vixen::AntSliderUtils
{
extern const std::array<std::array<BitBoard, Constants::SQUARE_NUMBER>, Constants::COLOR_NUMBER> pawnAttack;

extern const std::array<BitBoard, Constants::SQUARE_NUMBER> knightAttack;

extern const std::array<BitBoard, Constants::SQUARE_NUMBER> kingAttack;

template <AntiSliderDirections offset>
[[nodiscard]] constexpr auto Init8DirectionAttack() noexcept -> std::array<BitBoard, Constants::SQUARE_NUMBER>;

constexpr auto InitPawnAttack() noexcept
    -> std::array<std::array<BitBoard, Constants::SQUARE_NUMBER>, Constants::COLOR_NUMBER>;

#include "anti_slider.inl"

} // namespace Vixen::AntSliderUtils

#endif // VIXEN_ANTI_SLIDER_HPP_INCLUDED
