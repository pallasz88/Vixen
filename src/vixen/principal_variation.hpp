#ifndef VIXEN_PRINCIPAL_VARIATION_HPP_INCLUDED
#define VIXEN_PRINCIPAL_VARIATION_HPP_INCLUDED

#include <list>

#include "defs.hpp"
#include "move.hpp"

namespace vixen
{

struct PVEntry
{
    Move move;

    PositionKey positionKey;

    PVEntry() = default;

    explicit PVEntry(Move m, PositionKey key) : move(m), positionKey(key)
    {
    }
};

class PrincipalVariation
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

#endif // VIXEN_PRINCIPAL_VARIATION_HPP_INCLUDED
