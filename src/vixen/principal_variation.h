#ifndef VIXEN_PRINCIPALVARIATION_H
#define VIXEN_PRINCIPALVARIATION_H

#include <list>

#include "defs.h"

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
    std::list<PVEntry> elements;

    std::unordered_map<PositionKey, std::list<PVEntry>::iterator> hashTable;

    size_t capacity;
};

} // namespace Vixen

#endif // VIXEN_PRINCIPALVARIATION_H
