#ifndef VIXEN_PRINCIPALVARIATION_H
#define VIXEN_PRINCIPALVARIATION_H

#include <list>
#include <memory_resource>

#include "defs.h"
#include "move.h"

namespace Vixen
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

    void StorePVEntry(PVEntry &&);

    PVEntry GetPVEntry(PositionKey);

  private:
    size_t capacity;

    std::byte buffer[4096];

    std::pmr::monotonic_buffer_resource resource{buffer, sizeof buffer};

    std::pmr::list<PVEntry> elements{&resource};

    std::pmr::unordered_map<PositionKey, std::list<PVEntry>::iterator> hashTable{&resource};
};

} // namespace Vixen

#endif // VIXEN_PRINCIPALVARIATION_H
