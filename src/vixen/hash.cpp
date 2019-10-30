#include "hash.h"
#include "board.h"

#include <random>

namespace Vixen
{
    PieceHashKeys Hash::pieceHashKeys;

    SideHashKey Hash::sideHashKey;

    CastleHashKeys Hash::castleHashKeys;

    void Hash::InitZobristKeys()
    {
        for (int square = H1; square <= MAX_SQUARE_INDEX; ++square)
        {
            pieceHashKeys[square][enPassantKey] = GenerateBigRandom();
            for (const auto &pieceKey : pieceKeys)
                pieceHashKeys[square][pieceKey] = GenerateBigRandom();
        }

        sideHashKey = GenerateBigRandom();

        for (auto &castleHashKey : castleHashKeys)
            castleHashKey = GenerateBigRandom();

    }

    BitBoard Hash::GenerateBigRandom()
    {
        static std::mt19937_64 generator(0xDEADBEEF);
        return generator();
    }

    void Hash::ComputePositionKey(const Board &board)
    {
        positionKey = 0;

        for (int square = H1; square <= MAX_SQUARE_INDEX; ++square)
        {
            for (const auto &pieceKey : pieceKeys)
            {
                if (IsBitSet(board.GetBitBoard(pieceKey), square))
                {
                    HashPiece(square, pieceKey);
                    break;
                }
            }
        }

        auto enPassant = board.GetEnPassant();
        if (enPassant != EMPTY_BOARD)
            HashEnPassant(enPassant);

        if (board.IsWhiteToMove())
            HashSide();

        HashCastling(board.GetCastlingRights());
    }
}