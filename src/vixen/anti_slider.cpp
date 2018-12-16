#include "anti_slider.h"

namespace Vixen
{
    BitBoard knightAttack[SQUARE_NUMBER];

    BitBoard kingAttack[SQUARE_NUMBER];

    void InitKnightKingAttack()
    {
        const int knightOffset[8][2] =
                {{-2, -1},
                 {-2, 1},
                 {-1, -2},
                 {-1, 2},
                 {1,  -2},
                 {1,  2},
                 {2,  -1},
                 {2,  1}};
        const int kingOffset[8][2] =
                {{-1, -1},
                 {-1, 0},
                 {-1, 1},
                 {0,  -1},
                 {0,  1},
                 {1,  -1},
                 {1,  0},
                 {1,  1}};
        for (unsigned square = 0; square < SQUARE_NUMBER; ++square)
        {
            for (unsigned direction = 0; direction < 8; ++direction)
            {
                unsigned knightTarget = square % 8 + knightOffset[direction][0] +
                                        (square / 8 + knightOffset[direction][1]) * 8;
                unsigned kingTarget = square % 8 + kingOffset[direction][0] +
                                      (square / 8 + kingOffset[direction][1]) * 8;
                if (IsValidSquare(knightTarget % 8, knightTarget / 8))
                    knightAttack[square] |= 1ULL << knightTarget;


                if (IsValidSquare(kingTarget % 8, kingTarget / 8))
                    kingAttack[square] |= 1ULL << kingTarget;

            }
        }
    }
}

