#include "principal_variation.hpp"

namespace vixen
{
void PrincipalVariation::StorePVEntry(const PVEntry &entry)
{
    const auto &reference = hashTable.find(entry.positionKey);

    if (reference == hashTable.end())
    {
        if (hashTable.size() == capacity)
        {
            PVEntry lastUsed = elements.back();
            elements.pop_back();
            hashTable.erase(lastUsed.positionKey);
        }
    }

    else
        elements.erase(reference->second);

    elements.emplace_front(entry);
    hashTable[entry.positionKey] = begin(elements);
}

PVEntry PrincipalVariation::GetPVEntry(PositionKey key) const
{
    const auto entry = hashTable.find(key);

    if (entry != end(hashTable))
        return *entry->second;

    else
        return {};
}
} // namespace vixen