#ifndef VIXEN_BOARD_HPP_INCLUDED
#define VIXEN_BOARD_HPP_INCLUDED

#include "fixed_list.hpp"
#include "hash.hpp"
#include "move.hpp"

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

    explicit constexpr History(BitBoard en, unsigned cr, unsigned fi, Move lastMove, char moved, char captured,
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
    [[nodiscard]] constexpr BitBoard GetBitBoard(unsigned char piece) const
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

    [[nodiscard]] constexpr auto GetKiller(const int depth, const int index) const
    {
        return killer[static_cast<size_t>(depth)][static_cast<size_t>(index)];
    }

    constexpr void ResetKillers()
    {
        for (auto &k : killer)
            std::fill(std::begin(k), std::end(k), Move());
    }

    constexpr void UpdateKillers(const Move &move, const int depth)
    {
        killer[static_cast<unsigned>(depth)][1U] = killer[static_cast<unsigned>(depth)][0U];
        killer[static_cast<unsigned>(depth)][0U] = move;
    }

    constexpr void ResetHistory()
    {
        for (auto &h : historyHeuristic)
            std::fill(std::begin(h), std::end(h), 0);
    }

    constexpr void IncreaseHistoryValue(const int depth, const unsigned from, const unsigned to)
    {
        historyHeuristic[from][to] += static_cast<unsigned>(depth * depth);
    }

    [[nodiscard]] constexpr auto GetHistoryValue(const unsigned from, const unsigned to) const
    {
        return historyHeuristic[from][to];
    }

    [[nodiscard]] constexpr bool HasHeavyPieces() const
    {
        return IsWhiteToMove() ? static_cast<bool>(GetBitBoard('Q') | GetBitBoard('R'))
                               : static_cast<bool>(GetBitBoard('q') | GetBitBoard('r'));
    }

    /**
     * Prints the board to console.
     */
    void PrintBoard() const noexcept;

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
    constexpr void RemovePiece(unsigned int position, char pieceType) noexcept;

    /**
     * Adds given piece type to given position.
     * @param position
     * @param pieceType
     */
    constexpr void AddPiece(unsigned int position, char pieceType) noexcept;

    /**
     * Tries to move a pseudo-legal move.
     * If after the move turns out king is in check, function rolls back to
     * previous state. Warning: Make sure, that the move is at least pseudo-legal,
     * the function does not check it.
     * @param move
     * @return
     */
    [[nodiscard]] bool MakeMove(Move move) noexcept;

    [[nodiscard]] bool MakeMove(std::string_view move);

    constexpr void MakeNullMove() noexcept
    {
        whiteToMove = !whiteToMove;
    }

    constexpr void TakeNullMove() noexcept
    {
        whiteToMove = !whiteToMove;
    }

    /**
     * Rolls back to previous state of the board.
     */
    void TakeBack();

    /**
     * Returns a generator of pseudo-legal moves from given position.
     * @return pseudo-legal moves
     */
    template <uint8_t moveType>[[nodiscard]] FixedList<Move> GetMoveList() const noexcept;

    [[nodiscard]] constexpr bool IsRepetition() const noexcept
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
    std::array<std::array<Move, 2>, 64> killer{};

    std::array<std::array<unsigned, Constants::SQUARE_NUMBER>, Constants::SQUARE_NUMBER> historyHeuristic{};

    std::array<History, 1024> history{};

    std::array<char, Constants::SQUARE_NUMBER> pieceList{};

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
    [[nodiscard]] constexpr auto SplitFenPosition(std::string_view position) const noexcept;

    constexpr void MakeCapture(unsigned int to, char capturedPieceLetter, unsigned int moveType) noexcept;

    constexpr void MakeDoublePawnPush(unsigned int enPassantSquare) noexcept;

    constexpr void MoveCastlingWhiteRook(unsigned int from, unsigned int to) noexcept;

    constexpr void MoveCastlingBlackRook(unsigned int from, unsigned int to) noexcept;

    constexpr void UpdateCastlingRights(unsigned int from, unsigned int to) noexcept;
};
} // namespace Vixen

#endif // VIXEN_BOARD_HPP_INCLUDED