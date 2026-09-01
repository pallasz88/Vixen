#include "principal_variation.hpp"

#include "board.hpp"

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

FixedList<Move> PrincipalVariation::GetMoveList(int depth, Board &board) const
{
    FixedList<Move> moveList{};
    int ply = 0;

    while (ply < depth)
    {
        if (const auto bestMove = GetPVEntry(board.GetHash()).moveEntry; bestMove != 0U && board.MakeMove(bestMove))
        {
            ++ply;
            moveList.emplace_back(bestMove);
        }

        else
            break;
    }

    for (int j = 0; j < ply; ++j)
        board.TakeBack();

    return moveList;
}
} // namespace vixen