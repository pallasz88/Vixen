#pragma once

#include "hash.h"
#include "move.h"

namespace Vixen
{

class MoveGenerator;

/**
 * Used for recovering previous positions to start position.
 */
struct History
{
    BitBoard enPassant;

    PositionKey hash;

    unsigned castlingRights;

    unsigned fiftyMoves;

    Move move;

    char movedPiece;

    char capturedPiece;

    History() = default;

    explicit History(BitBoard en, unsigned cr, unsigned fi, Move lastMove, char moved, char captured,
                     PositionKey lastHash)
        : enPassant(en), hash(lastHash), castlingRights(cr), fiftyMoves(fi), move(lastMove), movedPiece(moved),
          capturedPiece(captured)
    {
    }

    [[nodiscard]] friend constexpr bool operator==(const History &history, PositionKey positionKey)
    {
        return history.hash == positionKey;
    }
};

/**
 * Holds the representation of chess board.
 */
class VIXEN_API Board
{
  public:
    /**
     * Constructor
     */
    Board();

    /**
     * Returns piece bitboard from bitBoards
     * @param piece
     * @return
     */
    [[nodiscard]] constexpr BitBoard GetBitBoard(char piece) const
    {
        return bitBoards[static_cast<unsigned>(GetPieceIndex(piece))];
    }

    /**
     * Returns piece square list.
     * @return
     */
    [[nodiscard]] constexpr auto GetPieceList() const
    {
        return pieceList;
    }

    /**
     *
     * @return
     */
    [[nodiscard]] constexpr auto GetHash() const
    {
        return hashBoard.GetHash();
    }

    /**
     * Returns if white is on move from given position.
     * @return whiteToMove
     */
    [[nodiscard]] constexpr bool IsWhiteToMove() const
    {
        return whiteToMove;
    }

    /**
     * Returns a bitboard for enpassant from given position.
     * @return enPassantBitBoard
     */
    [[nodiscard]] constexpr BitBoard GetEnPassant() const
    {
        return enPassantBitBoard;
    }

    /**
     * Returns castling rights from given position.
     * @return castlingRights
     */
    [[nodiscard]] constexpr unsigned GetCastlingRights() const
    {
        return castlingRights;
    }

    /**
     * Returns material balance on board
     */
    [[nodiscard]] constexpr int GetMaterialBalance() const
    {
        return material;
    }

    [[nodiscard]] constexpr unsigned GetFiftyMoveCounter() const
    {
        return fiftyMoves;
    }

    /**
     * Prints the board to console.
     */
    void PrintBoard() const;

    /**
     * Sets board from FEN position.
     *  FEN position:
     * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
     * @param position
     */
    void SetBoard(std::string_view position);

    /**
     * Removes given piece type from given position.
     * @param position
     * @param pieceType
     */
    constexpr void RemovePiece(unsigned int position, char pieceType);

    /**
     * Adds given piece type to given position.
     * @param position
     * @param pieceType
     */
    constexpr void AddPiece(unsigned int position, char pieceType);

    /**
     * Tries to move a pseudo-legal move.
     * If after the move turns out king is in check, function rolls back to
     * previous state. Warning: Make sure, that the move is at least pseudo-legal,
     * the function does not check it.
     * @param move
     * @return
     */
    bool MakeMove(Move move);

    bool MakeMove(std::string_view move);

    /**
     * Rolls back to previous state of the board.
     */
    void TakeBack();

    /**
     * Returns a generator of pseudo-legal moves from given position.
     * @return pseudo-legal moves
     */
    template <uint8_t moveType>[[nodiscard]] MoveGenerator CreateGenerator() const;

    [[nodiscard]] constexpr bool IsRepetition() const
    {
#ifdef __cpp_lib_constexpr_algorithms
        return std::find(begin(history) + historyPly - fiftyMoves, begin(history) + historyPly, hashBoard.GetHash()) !=
               begin(history) + historyPly;
#else
        return Vixen::find(begin(history) + historyPly - fiftyMoves, begin(history) + historyPly,
                           hashBoard.GetHash()) != begin(history) + historyPly;
#endif
    }

  private:
    std::array<History, 1024> history{};

    std::array<char, Constants::SQUARE_NUMBER> pieceList;

    BitBoards bitBoards;

    BitBoard enPassantBitBoard;

    Hash hashBoard;

    unsigned castlingRights;

    unsigned historyPly;

    unsigned fiftyMoves;

    int material;

    bool whiteToMove;

    constexpr void SumUpBitBoards();

    void AddHashBoard();

    constexpr void ParseFenPiecePart(std::string_view parsedPosition);

    constexpr void ParseSideToMovePart(std::string_view splittedFen);

    constexpr void ParseCastlingRightPart(std::string_view parsedPosition);

    template <size_t N, char delimiter = ' '>
    [[nodiscard]] constexpr auto SplitFenPosition(std::string_view position) const;

    void ClearHistory();

    constexpr void MakeCapture(unsigned int to, char capturedPieceLetter, unsigned int moveType);

    constexpr void MakeDoublePawnPush(unsigned int enPassantSquare);

    constexpr void MoveCastlingWhiteRook(unsigned int from, unsigned int to);

    constexpr void MoveCastlingBlackRook(unsigned int from, unsigned int to);

    constexpr void UpdateCastlingRights(unsigned int from, unsigned int to);
};
} // namespace Vixen