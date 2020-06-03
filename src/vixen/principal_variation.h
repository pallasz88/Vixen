#ifndef VIXEN_PRINCIPALVARIATION_H
#define VIXEN_PRINCIPALVARIATION_H

#include "defs.h"

namespace Vixen
{

    struct PVEntry
    {
        Move move;

        PositionKey positionKey;

        PVEntry() = default;

        explicit PVEntry(Move m, PositionKey key)
                : move(m), positionKey(key)
        {}
    };

    class PrincipalVariation
    {
    public:

        void ClearTable();

        void StorePVEntry(PVEntry &&);

        PVEntry GetPVEntry(PositionKey key);

    private:

        static constexpr int megabyte = 0x200000;

        std::unordered_map<PositionKey, PVEntry> hashTable;

    };

}


#endif //VIXEN_PRINCIPALVARIATION_H
