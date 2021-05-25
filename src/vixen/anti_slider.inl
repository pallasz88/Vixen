#ifndef ANTI_SLIDER_INL_INCLUDED
#define ANTI_SLIDER_INL_INCLUDED

namespace Vixen::AntSliderUtils
{
template <AntiSliderDirections offset>
[[nodiscard]] constexpr auto Init8DirectionAttack() noexcept -> std::array<BitBoard, Constants::SQUARE_NUMBER>
{
    std::array<BitBoard, Constants::SQUARE_NUMBER> eightBB{};
    for (unsigned square = 0; square < Constants::SQUARE_NUMBER; ++square)
    {
        for (unsigned direction = 0; direction < 8; ++direction)
        {
            const int fileOffset = offset[direction][0];
            const int rankOffset = offset[direction][1];
            const int eightFile = static_cast<int>(square % 8) + fileOffset;
            const int eightRank = static_cast<int>(square / 8) + rankOffset;
            if (IsValidCoordinate(eightFile, eightRank))
                SetBit(eightBB[square], static_cast<unsigned>(8 * eightRank + eightFile));
        }
    }
    return eightBB;
}

constexpr auto InitPawnAttack() noexcept
    -> std::array<std::array<BitBoard, Constants::SQUARE_NUMBER>, Constants::COLOR_NUMBER>
{
    std::array<std::array<BitBoard, Constants::SQUARE_NUMBER>, Constants::COLOR_NUMBER> pawnBB{};
    for (unsigned square = 0; square < Constants::SQUARE_NUMBER; ++square)
    {
        for (unsigned direction = 0; direction < 2; ++direction)
        {
            int pawnFile = static_cast<int>(square % 8) + Constants::pawnDirections[direction][0];
            int pawnRank = static_cast<int>(square / 8) + Constants::pawnDirections[direction][1];
            if (IsValidCoordinate(pawnFile, pawnRank))
                SetBit(pawnBB[static_cast<int>(Colors::WHITE)][square], static_cast<unsigned>(8 * pawnRank + pawnFile));

            pawnFile = static_cast<int>(square % 8) - Constants::pawnDirections[direction][0];
            pawnRank = static_cast<int>(square / 8) - Constants::pawnDirections[direction][1];
            if (IsValidCoordinate(pawnFile, pawnRank))
                SetBit(pawnBB[static_cast<int>(Colors::BLACK)][square], static_cast<unsigned>(8 * pawnRank + pawnFile));
        }
    }
    return pawnBB;
}
} // namespace Vixen::AntSliderUtils
#endif