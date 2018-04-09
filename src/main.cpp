#include "defs.h"
#include "Board.h"
#include "MoveGenerator.h"
#include "Hash.h"

#include <iostream>

int main()
{
#ifndef DEBUG
    CURRENT_BOARD()->PrintBoard();
    CURRENT_MOVES();
    CURRENT_BOARD()->CleanUp();
    CURRENT_MOVES()->CleanUp();
#else
    Vixen::Hash::GetInstance()->GetHash();
    CURRENT_MOVES();
    CURRENT_BOARD()->CleanUp();
    CURRENT_MOVES()->CleanUp();
    Vixen::Hash::GetInstance()->CleanUp();
#endif //DEBUG
    return 0;
}
