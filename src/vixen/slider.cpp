#include "slider.hpp"

namespace vixen::slider_utils
{

const std::array<Magic, Constants::SQUARE_NUMBER> BishopTable = initMagics<BishopSliderInfo>();

const std::array<Magic, Constants::SQUARE_NUMBER> RookTable = initMagics<RookSliderInfo>();

} // namespace vixen::slider_utils
