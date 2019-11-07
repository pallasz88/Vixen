#include "anti_slider.h"

namespace Vixen::AntSliderUtils
{
    BitBoard pawnAttack[Constants::COLOR_NUMBER][Constants::SQUARE_NUMBER];

    BitBoard knightAttack[Constants::SQUARE_NUMBER];

    BitBoard kingAttack[Constants::SQUARE_NUMBER];

    void InitKnightKingAttack()
    {
        for (unsigned square = 0; square < Constants::SQUARE_NUMBER; ++square)
        {
            for (unsigned direction = 0; direction < 8; ++direction)
            {
                const int knightFile = static_cast<int>(square % 8) + Constants::knightOffset[direction][0];
                const int knightRank = static_cast<int>(square / 8) + Constants::knightOffset[direction][1];
                const int kingFile   = static_cast<int>(square % 8) + Constants::kingOffset[direction][0];
                const int kingRank   = static_cast<int>(square / 8) + Constants::kingOffset[direction][1];
                if (IsValidCoordinate(knightFile, knightRank))
                    SetBit(knightAttack[square], static_cast<unsigned>(8 * knightRank + knightFile));

                if (IsValidCoordinate(kingFile, kingRank))
                    SetBit(kingAttack[square], static_cast<unsigned>(8 * kingRank + kingFile));

            }
        }
    }

    void InitPawnAttack()
    {
        for (unsigned square = 0; square < Constants::SQUARE_NUMBER; ++square)
        {
            for (unsigned direction = 0; direction < 2; ++direction)
            {
                int pawnFile = static_cast<int>(square % 8) + Constants::pawnDirections[direction][0];
                int pawnRank = static_cast<int>(square / 8) + Constants::pawnDirections[direction][1];
                if (IsValidCoordinate(pawnFile, pawnRank))
                    SetBit(pawnAttack[static_cast<int>(Colors::WHITE)][square],
                                      static_cast<unsigned>(8 * pawnRank + pawnFile));

                pawnFile = static_cast<int>(square % 8) - Constants::pawnDirections[direction][0];
                pawnRank = static_cast<int>(square / 8) - Constants::pawnDirections[direction][1];
                if (IsValidCoordinate(pawnFile, pawnRank))
                    SetBit(pawnAttack[static_cast<int>(Colors::BLACK)][square],
                                      static_cast<unsigned>(8 * pawnRank + pawnFile));
            }
        }
    }
}

