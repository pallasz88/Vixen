#pragma once

#include "defs.h"

namespace Vixen::AntSliderUtils
{
    extern BitBoard pawnAttack[Constants::COLOR_NUMBER][Constants::SQUARE_NUMBER];

    extern BitBoard knightAttack[Constants::SQUARE_NUMBER];

    extern BitBoard kingAttack[Constants::SQUARE_NUMBER];

    void InitKnightKingAttack();

    void InitPawnAttack();
}
