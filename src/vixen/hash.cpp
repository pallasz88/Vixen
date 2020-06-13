#include "hash.h"

#include "board.h"

namespace Vixen
{

Hash::Keys Hash::zobristKeys;

void Hash::ComputePositionKey(const Board &board)
{
    positionKey = 0;

    for (unsigned square = H1; square <= Constants::MAX_SQUARE_INDEX; ++square)
    {
        for (const auto &pieceKey : Constants::pieceKeys)
        {
            if (IsBitSet(board.GetBitBoard(pieceKey), square))
            {
                HashPiece(square, pieceKey);
                break;
            }
        }
    }

    auto enPassant = board.GetEnPassant();
    if (enPassant != Constants::EMPTY_BOARD)
        HashEnPassant(enPassant);

    if (board.IsWhiteToMove())
        HashSide();

    HashCastling(board.GetCastlingRights());
}
} // namespace Vixen