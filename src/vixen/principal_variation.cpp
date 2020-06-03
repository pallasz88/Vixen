#include "principal_variation.h"

namespace Vixen
{
    void PrincipalVariation::ClearTable()
    {
        hashTable.clear();
    }

    void PrincipalVariation::StorePVEntry(PVEntry &&entry)
    {
        const auto[f, inserted] = hashTable.emplace(entry.positionKey, entry);

        if (!inserted)
            f->second.move = entry.move;

    }

    PVEntry PrincipalVariation::GetPVEntry(PositionKey key)
    {
        const auto entry = hashTable.find(key);

        if (entry != hashTable.end())
            return entry->second;

        else
            return {};
    }
}