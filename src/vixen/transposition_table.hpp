#ifndef SRC_VIXEN_TRANSPOSITION_TABLE_HPP_
#define SRC_VIXEN_TRANSPOSITION_TABLE_HPP_

#include <cstddef>
#include <optional>
#include <vector>

#include "defs.hpp"
#include "move.hpp"

namespace vixen
{

enum class Bound : uint8_t
{
    EXACT,
    LOWER,
    UPPER
};

struct TranspositionEntry
{
    PositionKey key{};
    Move move{};
    int score{};
    int depth{-1};
    Bound bound{Bound::EXACT};
};

class TranspositionTable
{
  public:
    explicit TranspositionTable(std::size_t capacity) : entries(capacity)
    {
    }

    void Clear() noexcept
    {
        for (auto &entry : entries)
            entry = {};
    }

    [[nodiscard]] std::optional<TranspositionEntry> Probe(PositionKey key) const noexcept
    {
        if (entries.empty())
            return std::nullopt;

        const auto &entry = entries[index(key)];
        if (entry.key != key || entry.depth < 0)
            return std::nullopt;

        return entry;
    }

    void Store(PositionKey key, Move move, int score, int depth, Bound bound) noexcept
    {
        if (entries.empty())
            return;

        auto &entry = entries[index(key)];
        if (entry.depth <= depth || entry.key != key)
            entry = TranspositionEntry{key, move, score, depth, bound};
    }

  private:
    std::vector<TranspositionEntry> entries;

    [[nodiscard]] std::size_t index(PositionKey key) const noexcept
    {
        return static_cast<std::size_t>(key % entries.size());
    }
};

} // namespace vixen

#endif // SRC_VIXEN_TRANSPOSITION_TABLE_HPP_
