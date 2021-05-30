#ifndef VIXEN_MOVE_HPP_INCLUDED
#define VIXEN_MOVE_HPP_INCLUDED

#include "defs.hpp"

namespace Vixen
{

using Representation = unsigned;

class Move
{
  public:
    Move() = default;

    explicit Move(unsigned move) : represantation(move), score(0)
    {
    }

    constexpr Move(unsigned from, unsigned to, uint8_t moveType)
        : represantation(static_cast<unsigned>(moveType << 12U) | to << 6U | from), score(0)
    {
    }

    ~Move() = default;

    constexpr Move(const Move &) = default;

    constexpr Move &operator=(const Move &) = default;

    constexpr operator bool() const
    {
        return represantation != 0;
    }

    constexpr void SetScore(unsigned newScore) noexcept
    {
        score = newScore;
    }

    constexpr unsigned GetScore() const noexcept
    {
        return score;
    }

    constexpr unsigned GetFromSquare() const noexcept
    {
        return represantation & 0x3FU;
    }

    constexpr unsigned GetToSquare() const noexcept
    {
        return (represantation >> 6U) & 0x3FU;
    }

    constexpr unsigned GetMoveType() const noexcept
    {
        return represantation >> 12U;
    }

    constexpr unsigned RemoveMoveType() const noexcept
    {
        return represantation & 0xFFFU;
    }

    constexpr bool operator==(const Move &rhs) const noexcept
    {
        return rhs.represantation == represantation;
    }

    constexpr bool operator==(const Representation rhs) const noexcept
    {
        return this->represantation == rhs;
    }

    constexpr bool operator!=(const Representation rhs) const noexcept
    {
        return this->represantation != rhs;
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
        const std::string from = SquareToNotation(move.represantation & 0x3FU);
        const std::string to = SquareToNotation((move.represantation & 0xFC0U) >> 6U);
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
    Representation represantation;

    unsigned score;
};

} // namespace Vixen

#endif // VIXEN_MOVE_HPP_INCLUDED