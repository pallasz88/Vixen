#pragma once

#include "defs.h"

namespace Vixen
{
    extern BitBoard pawnAttack[COLOR_NUMBER][SQUARE_NUMBER];

    extern BitBoard knightAttack[SQUARE_NUMBER];

    extern BitBoard kingAttack[SQUARE_NUMBER];

    void InitKnightKingAttack();

    void InitPawnAttack();
}
