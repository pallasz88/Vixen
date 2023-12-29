#ifndef VIXEN_MOVE_GENERATOR_HPP_INCLUDED
#define VIXEN_MOVE_GENERATOR_HPP_INCLUDED

#include <cassert>

#include "anti_slider.hpp"
#include "board.hpp"
#include "defs.hpp"
#include "fixed_list.hpp"
#include "slider.hpp"

namespace vixen
{
/**
 * Generates all pseudo-legal moves from Board class position.
 */
class VIXEN_API MoveGenerator
{
  public:
    /**
     * Returns pseudo-legal move list.
     * @return moveList
     */
    [[nodiscard]] constexpr auto GetMoveList() const noexcept
    {
        return moveList;
    }

    [[nodiscard]] auto GetLegalMoveList(Board &board) const noexcept;

    /**
     * Fills moveList by generating all pseudo moves.
     * @tparam sideToMove
     * @param board
     */
    template <Colors sideToMove, MoveTypes moveType> void GenerateMoves(const Board &board) noexcept;

  private:
    FixedList<Move> moveList;

    template <Colors sideToMove> void GenerateCaptureMoves(const Board &board) noexcept;

    template <Colors sideToMove> void GenerateQuietMoves(const Board &board) noexcept;

    template <Slider slider, uint8_t moveType>
    void GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets) noexcept;

    template <uint8_t moveType>
    constexpr void GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces,
                                           const std::array<BitBoard, Constants::SQUARE_NUMBER> &attackBoard) noexcept;

    constexpr void GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType) noexcept;

    constexpr void GeneratePawnPromotionMoves(int offset, BitBoard promotion) noexcept;

    constexpr void GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion) noexcept;

    template <Colors sideToMove> constexpr void GenerateCastlingMoves(const Board &board) noexcept;
};

namespace Check
{
template <Colors sideToMove> constexpr bool IsSquareAttacked(unsigned int square, const Board &board) noexcept
{
    assert(square <= static_cast<unsigned>(vixen::Squares::A8));
    assert(square >= static_cast<unsigned>(vixen::Squares::H1));
    const auto blockers = ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX);
    const auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::BLACK_PAWN_INDEX)
                                                   : board.GetBitBoard(Constants::WHITE_PAWN_INDEX);
    const auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::BLACK_KNIGHT_INDEX)
                                                     : board.GetBitBoard(Constants::WHITE_KNIGHT_INDEX);
    const auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::BLACK_BISHOP_INDEX)
                                                     : board.GetBitBoard(Constants::WHITE_BISHOP_INDEX);
    const auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::BLACK_ROOK_INDEX)
                                                   : board.GetBitBoard(Constants::WHITE_ROOK_INDEX);
    const auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::BLACK_QUEEN_INDEX)
                                                    : board.GetBitBoard(Constants::WHITE_QUEEN_INDEX);
    const auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::BLACK_KING_INDEX)
                                                   : board.GetBitBoard(Constants::WHITE_KING_INDEX);

    return anti_slider_utils::pawnAttack[static_cast<int>(sideToMove)][square] & pawns ||
           anti_slider_utils::knightAttack[square] & knights ||
           slider_utils::GetBishopAttack(square, blockers) & (bishops | queens) ||
           slider_utils::GetRookAttack(square, blockers) & (rooks | queens) ||
           anti_slider_utils::kingAttack[square] & kings;
}

/**
 * This function returns if king is in check.
 * @tparam sideToMove
 * @param board
 * @return Check on board
 */
template <Colors sideToMove> constexpr bool IsInCheck(const Board &board) noexcept
{
    const auto kingBoard = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_KING_INDEX)
                                                       : board.GetBitBoard(Constants::BLACK_KING_INDEX);
    const auto kingSquare = TrailingZeroCount(kingBoard);
    return IsSquareAttacked<sideToMove>(kingSquare, board);
}
} // namespace Check

namespace Test
{
/**
 * Helper function to validate move generation:
 *  - Perft method: https://www.chessprogramming.org/Perft
 *  - Perft results: https://www.chessprogramming.org/Perft_Results
 * @param depth
 * @param board
 * @return visited node number
 */
VIXEN_API BitBoard PerftTest(int depth, Board &board) noexcept;

VIXEN_API BitBoard Perft(int depth, Board &board, BitBoard &leafs) noexcept;
} // namespace Test
} // namespace vixen

#endif // VIXEN_MOVE_GENERATOR_HPP_INCLUDED