#ifndef SRC_VIXEN_PRINCIPAL_VARIATION_HPP_
#define SRC_VIXEN_PRINCIPAL_VARIATION_HPP_

#include <list>
#include <unordered_map>

#include "defs.hpp"
#include "move.hpp"

namespace vixen
{

struct PVEntry
{
    PositionKey positionKey{};

    Move moveEntry{};

    PVEntry() = default;

    explicit PVEntry(Move m, PositionKey key) : positionKey(key), moveEntry(m)
    {
    }

    [[nodiscard]] constexpr bool operator==(const PVEntry &rhs) const
    {
        if (moveEntry == rhs.moveEntry && positionKey == rhs.positionKey)
            return true;

        return false;
    }
};

class VIXEN_API PrincipalVariation
{
  public:
    explicit PrincipalVariation(size_t n) : capacity(n)
    {
    }

    void StorePVEntry(const PVEntry &);

    PVEntry GetPVEntry(PositionKey);

  private:
    size_t capacity;

    std::list<PVEntry> elements;

    std::unordered_map<PositionKey, std::list<PVEntry>::iterator> hashTable;
};

} // namespace vixen

#endif // SRC_VIXEN_PRINCIPAL_VARIATION_HPP_
