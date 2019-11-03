#include "hash.h"
#include "board.h"

namespace Vixen
{

    Hash::Keys Hash::zobristKeys;

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