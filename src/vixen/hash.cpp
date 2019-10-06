#include "hash.h"
#include "board.h"

#include <iostream>
#include <random>

namespace Vixen
{
    Hash::Hash(const Board &board)
    {
        InitZobristKeys();
        ComputePositionKey(board);
    }

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
        {
            castleHashKey = GenerateBigRandom();
        }

    }

    BitBoard Hash::GenerateBigRandom()
    {
        static std::default_random_engine generator(std::random_device{}());
        static std::uniform_int_distribution<uint64_t> distribution(0, UINTMAX_MAX);
        return distribution(generator);
    }

    void Hash::ComputePositionKey(const Board &board)
    {
        positionKey = 0;

        for (int square = H1; square <= MAX_SQUARE_INDEX; ++square)
        {
            for (const auto &pieceKey : pieceKeys)
            {
                if (IsBitSet(board.GetBitBoard(pieceKey), square))
                    HashPiece(square, pieceKey);
            }
        }

        auto enPassant = board.GetEnPassant();
        if (enPassant != EMPTY_BOARD)
            HashEnPassant(enPassant);

        if (board.IsWhiteToMove())
            HashSide();

        HashCastling(board);
    }
}