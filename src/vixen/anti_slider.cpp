#include "anti_slider.hpp"

namespace vixen::anti_slider_utils
{
const std::array<std::array<BitBoard, Constants::SQUARE_NUMBER>, Constants::COLOR_NUMBER> pawnAttack = InitPawnAttack();

const std::array<BitBoard, Constants::SQUARE_NUMBER> knightAttack = Init8DirectionAttack<Constants::knightOffset>();

const std::array<BitBoard, Constants::SQUARE_NUMBER> kingAttack = Init8DirectionAttack<Constants::kingOffset>();
} // namespace vixen::anti_slider_utils
