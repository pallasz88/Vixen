#include "anti_slider.h"

namespace Vixen
{
    BitBoard pawnAttack[COLOR_NUMBER][SQUARE_NUMBER];

    BitBoard knightAttack[SQUARE_NUMBER];

    BitBoard kingAttack[SQUARE_NUMBER];

    void InitKnightKingAttack()
    {
        AntiSliderDirections knightOffset = {{{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}}};
        AntiSliderDirections kingOffset = {{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};
        for (auto square = 0; square < SQUARE_NUMBER; ++square)
        {
            for (int direction = 0; direction < 8; ++direction)
            {
                int knightFile = square % 8 + knightOffset[direction][0];
                int knightRank = square / 8 + knightOffset[direction][1];
                int kingFile = square % 8 + kingOffset[direction][0];
                int kingRank = square / 8 + kingOffset[direction][1];
                if (IsValidCoordinate(knightFile, knightRank))
                    knightAttack[square] |= 1ULL << (8 * knightRank + knightFile);

                if (IsValidCoordinate(kingFile, kingRank))
                    kingAttack[square] |= 1ULL << (8 * kingRank + kingFile);

            }
        }
    }

    void InitPawnAttack()
    {
        PawnDirections pawnDirections = {{{-1, 1}, {1, 1}}};
        for (auto square = 0; square < SQUARE_NUMBER; ++square)
        {
            for (int direction = 0; direction < 2; ++direction)
            {
                int pawnFile = square % 8 + pawnDirections[direction][0];
                int pawnRank = square / 8 + pawnDirections[direction][1];
                if (IsValidCoordinate(pawnFile, pawnRank))
                    pawnAttack[static_cast<int>(Colors::WHITE)][square] |= 1ULL << (8 * pawnRank + pawnFile);

                pawnFile = square % 8 - pawnDirections[direction][0];
                pawnRank = square / 8 - pawnDirections[direction][1];
                if (IsValidCoordinate(pawnFile, pawnRank))
                    pawnAttack[static_cast<int>(Colors::BLACK)][square] |= 1ULL << (8 * pawnRank + pawnFile);

            }
        }
    }
}

