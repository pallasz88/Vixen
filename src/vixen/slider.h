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


    struct Magic
    {
        BitBoard *attacks;  // pointer to attack_table for each particular square

        BitBoard mask;      // to mask relevant squares of both lines (no outer squares)

        BitBoard magic;     // magic 64-bit factor

        int shift;          // shift right

    };

    class SliderAttacks{

    public:

        explicit SliderAttacks(BitBoard occupied);

        BitBoard GetBishopAttack(unsigned square, BitBoard occupied);

        BitBoard GetRookAttack(unsigned square, BitBoard occupied);

    private:

        void InitMagics(BitBoard occupied);

        Magic BishopTable[SQUARE_NUMBER];

        Magic RookTable[SQUARE_NUMBER];

        int GetIndex(BitBoard occupied, const Magic &table);

        template <Slider slider>
        void InitSlidingAttack(int square, BitBoard occupied);

        BitBoard SlidingAttack(int square, SliderDirections directions, BitBoard occupied);
    };


}
