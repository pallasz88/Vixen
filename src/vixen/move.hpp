#ifndef VIXEN_MOVE_HPP_INCLUDED
#define VIXEN_MOVE_HPP_INCLUDED

#include "defs.hpp"

namespace vixen
{

using Representation = unsigned;

class Move
{
  public:
    constexpr Move() = default;

    explicit Move(unsigned move) : representation(move)
    {
    }

    constexpr Move(unsigned from, unsigned to, uint8_t moveType)
        : representation(static_cast<unsigned>(moveType << 12U) | to << 6U | from)
    {
    }

    ~Move() = default;

    constexpr Move(const Move &) = default;

    constexpr Move &operator=(const Move &) = default;

    constexpr explicit operator bool() const
    {
        return representation != 0;
    }

    constexpr void SetScore(unsigned newScore) noexcept
    {
        score = newScore;
    }

    [[maybe_unused]] [[nodiscard]] constexpr unsigned GetScore() const noexcept
    {
        return score;
    }

    [[nodiscard]] constexpr unsigned GetFromSquare() const noexcept
    {
        return representation & 0x3FU;
    }

    [[nodiscard]] constexpr unsigned GetToSquare() const noexcept
    {
        return (representation >> 6U) & 0x3FU;
    }

    [[nodiscard]] constexpr unsigned GetMoveType() const noexcept
    {
        return representation >> 12U;
    }

    [[nodiscard]] constexpr unsigned RemoveMoveType() const noexcept
    {
        return representation & 0xFFFU;
    }

    constexpr bool operator==(const Move &rhs) const noexcept
    {
        return rhs.representation == representation;
    }

    constexpr bool operator==(const Representation rhs) const noexcept
    {
        return this->representation == rhs;
    }

    constexpr bool operator!=(const Representation rhs) const noexcept
    {
        return this->representation != rhs;
    }

    constexpr bool operator<(const Move &rhs) const noexcept
    {
        return score < rhs.score;
    }

    constexpr bool operator>(const Move &rhs) const noexcept
    {
        return score > rhs.score;
    }

    friend std::ostream &operator<<(std::ostream &os, const Move &move) noexcept
    {
        const std::string from = SquareToNotation(move.representation & 0x3FU);
        const std::string to = SquareToNotation((move.representation & 0xFC0U) >> 6U);
        os << from << to;
        return os;
    }

    static constexpr int NotationToSquare(const std::string_view &notation) noexcept
    {
        if (notation.at(0) < 'a' || notation.at(0) > 'h' || notation.at(1) < '1' || notation.at(1) > '8')
            return -1;
        return 7 - (notation.at(0) - 'a') + 8 * (notation.at(1) - '1');
    }

    static std::string SquareToNotation(unsigned square) noexcept
    {
        std::string notation;
        notation.push_back(static_cast<char>(7 - square % 8 + 'a'));
        notation.push_back(static_cast<char>(square / 8 + '1'));
        return notation;
    }

  private:
    Representation representation{};

    unsigned score{0U};
};

} // namespace vixen

#endif // VIXEN_MOVE_HPP_INCLUDED