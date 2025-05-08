#ifndef SRC_VIXEN_PRINCIPAL_VARIATION_HPP_
#define SRC_VIXEN_PRINCIPAL_VARIATION_HPP_

#include "defs.hpp"
#include "move.hpp"

namespace vixen
{

struct TranspositionTableEntry
{
    PositionKey positionKey{};
    Move moveEntry{};
    int score{};
    int depth{};
    int flag{}; // 0 - exact, 1 - lower bound, 2 - upper bound

    TranspositionTableEntry() = default;

    TranspositionTableEntry(PositionKey key, Move m, int s, int d, int f)
        : positionKey(key), moveEntry(m), score(s), depth(d), flag(f)
    {
    }

    [[nodiscard]] constexpr auto operator<=>(const TranspositionTableEntry &other) const noexcept
    {
        return positionKey <=> other.positionKey;
    }

    [[nodiscard]] constexpr bool operator==(const TranspositionTableEntry &other) const noexcept
    {
        return positionKey == other.positionKey;
    }

    [[nodiscard]] constexpr bool operator!=(const TranspositionTableEntry &other) const noexcept
    {
        return positionKey != other.positionKey;
    }
};

class VIXEN_API PrincipalVariation
{
  public:
    explicit PrincipalVariation(size_t n) : capacity(n), table(n)
    {
    }

    constexpr void StorePVEntry(const TranspositionTableEntry &entry)
    {
        auto &slot = table[entry.positionKey % capacity];
        // Replace if new entry is deeper or slot is empty
        if (slot.positionKey == 0ULL || entry.depth >= slot.depth)
            slot = entry;
    }

    [[nodiscard]] constexpr TranspositionTableEntry GetPVEntry(PositionKey key) const
    {
        const auto &slot = table[key % capacity];
        if (slot.positionKey == key)
            return slot;
        return {};
    }

  private:
    std::size_t capacity;
    std::vector<TranspositionTableEntry> table;
};

} // namespace vixen

#endif // SRC_VIXEN_PRINCIPAL_VARIATION_HPP_
