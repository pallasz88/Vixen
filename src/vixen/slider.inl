#ifndef VIXEN_SLIDER_INL_INCLUDED
#define VIXEN_SLIDER_INL_INCLUDED

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
    BitBoard edges = ((FILEA | FILEH) & ~(FILEH << square % 8)) | ((RANK1 | RANK8) & ~(RANK1 << 8 * (square / 8)));
    auto occupied = Constants::EMPTY_BOARD;

    table[square].magic = slider == Slider::BISHOP ? Constants::BishopMagic[square] : Constants::RookMagic[square];
    table[square].mask = SlidingAttack(square, directions, occupied) & ~edges;
    table[square].shift = Constants::SQUARE_NUMBER - PopCount(table[square].mask);

    if (square != Constants::MAX_SQUARE_INDEX)
        table[square + 1].attacks = table[square].attacks + (1ULL << PopCount(table[square].mask));

    do
    {
        unsigned index = GetIndex(occupied, table[square]);
        table[square].attacks[index] = SlidingAttack(square, directions, occupied);
        occupied = (occupied - table[square].mask) & table[square].mask;
    } while (occupied);
}

inline void InitMagics() noexcept
{
    static std::array<BitBoard, Constants::BISHOP_ATTACK_TABLE_SIZE> bishopAttacks{};
    static std::array<BitBoard, Constants::ROOK_ATTACK_TABLE_SIZE> rookAttacks{};
    RookTable[0].attacks = &rookAttacks[0];
    BishopTable[0].attacks = &bishopAttacks[0];
    for (unsigned square = 0; square < Constants::SQUARE_NUMBER; ++square)
    {
        InitSlidingAttack<Slider::ROOK>(square, Constants::rookDirections, RookTable);
        InitSlidingAttack<Slider::BISHOP>(square, Constants::bishopDirections, BishopTable);
    }
}
#endif // VIXEN_SLIDER_INL_INCLUDED