#ifndef VIXEN_SLIDER_INL_INCLUDED
#define VIXEN_SLIDER_INL_INCLUDED

inline BitBoard GetBishopAttack(unsigned int square, BitBoard occupied) noexcept
{
    return BishopTable[square].GetAttack(occupied);
}

inline BitBoard GetRookAttack(unsigned int square, BitBoard occupied) noexcept
{
    return RookTable[square].GetAttack(occupied);
}

constexpr void GetNextCoordinate(int &file, int &rank, const Direction &direction) noexcept
{
    file += direction[0], rank += direction[1];
}

constexpr BitBoard SlidingAttack(unsigned int square, SliderDirections directions, BitBoard occupied) noexcept
{
    auto attacks = Constants::EMPTY_BOARD;
    for (const auto &direction : directions)
    {
        for (int file = static_cast<int>(square / 8) + direction[0], rank = static_cast<int>(square % 8) + direction[1];
             IsValidCoordinate(file, rank); GetNextCoordinate(file, rank, direction))
        {
            SetBit(attacks, static_cast<unsigned>(8 * file + rank));
            if (IsBitSet(occupied, static_cast<unsigned>(8 * file + rank)))
                break;
        }
    }

    return attacks;
}

template <Slider slider>
constexpr void InitSlidingAttack(unsigned int square, SliderDirections directions,
                                 std::array<Magic, Constants::SQUARE_NUMBER> &table) noexcept
{
    BitBoard edges = ((static_cast<BitBoard>(Files::FILEA) | static_cast<BitBoard>(Files::FILEH)) &
                      ~(static_cast<BitBoard>(Files::FILEH) << square % 8)) |
                     ((static_cast<BitBoard>(Ranks::RANK1) | static_cast<BitBoard>(Ranks::RANK8)) &
                      ~(static_cast<BitBoard>(Ranks::RANK1) << 8 * (square / 8)));
    auto occupied = Constants::EMPTY_BOARD;

    auto &magics = table[square];
    magics.magic = slider == Slider::BISHOP ? Constants::BishopMagic[square] : Constants::RookMagic[square];
    magics.mask = SlidingAttack(square, directions, occupied) & ~edges;
    magics.shift = Constants::SQUARE_NUMBER - PopCount(magics.mask);

    if (square != Constants::MAX_SQUARE_INDEX)
        table[square + 1].attacks = magics.attacks + (1ULL << PopCount(magics.mask));

    do
    {
        unsigned index = magics.GetIndex(occupied);
        magics.attacks[index] = SlidingAttack(square, directions, occupied);
        occupied = (occupied - magics.mask) & magics.mask;
    } while (occupied);
}
#endif // VIXEN_SLIDER_INL_INCLUDED