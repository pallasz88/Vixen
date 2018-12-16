#pragma once

#include <cstring>
#include "defs.h"

namespace Vixen
{
    extern BitBoard BishopAttacks[BishopAttackTableSize];

    extern BitBoard RookAttacks[RookAttackTableSize];

    enum class Slider
    {
        BISHOP, ROOK
    };

    template<Slider>
    class Magic
    {
        BitBoard *attacks;  // pointer to attack_table for each particular square

        BitBoard mask;      // to mask relevant squares of both lines (no outer squares)

        BitBoard magic;     // magic 64-bit factor

        unsigned shift;     // shift right

        BitBoard GetIndex(BitBoard occupied);

        void InitSlidingAttack(unsigned square);

        BitBoard SlidingAttack(unsigned square);

    public:

        static BitBoard GetAttack(unsigned square, BitBoard occupied);

        friend void InitMagics();
    };

    void InitMagics();

    extern Magic<Slider::BISHOP> BishopTable[SQUARE_NUMBER];

    extern Magic<Slider::ROOK> RookTable[SQUARE_NUMBER];
}
