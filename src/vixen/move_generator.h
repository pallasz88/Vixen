#pragma once

#include <cassert>

#include "anti_slider.h"
#include "board.h"
#include "fixed_list.h"
#include "slider.h"

namespace Vixen
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
    template <Colors sideToMove, uint8_t moveType> void GenerateMoves(const Board &board) noexcept;

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
    assert(square <= A8);
    assert(square >= H1);
    const auto blockers = ~board.GetBitBoard(' ');
    const auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard('p') : board.GetBitBoard('P');
    const auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard('n') : board.GetBitBoard('N');
    const auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard('b') : board.GetBitBoard('B');
    const auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard('r') : board.GetBitBoard('R');
    const auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard('q') : board.GetBitBoard('Q');
    const auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard('k') : board.GetBitBoard('K');

    return AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)][square] & pawns ||
           AntSliderUtils::knightAttack[square] & knights ||
           SliderUtils::GetBishopAttack(square, blockers) & (bishops | queens) ||
           SliderUtils::GetRookAttack(square, blockers) & (rooks | queens) ||
           AntSliderUtils::kingAttack[square] & kings;
}

/**
 * This function returns if king is in check.
 * @tparam sideToMove
 * @param board
 * @return Check on board
 */
template <Colors sideToMove> constexpr bool IsInCheck(const Board &board) noexcept
{
    const auto kingBoard = sideToMove == Colors::WHITE ? board.GetBitBoard('K') : board.GetBitBoard('k');
    const auto kingSquare = TrailingZeroCount(kingBoard);
    return IsSquareAttacked<sideToMove>(kingSquare, board);
}
} // namespace Check

namespace Test
{
extern BitBoard leafs;

/**
 * Helper function to validate move generation:
 *  - Perft method: https://www.chessprogramming.org/Perft
 *  - Perft results: https://www.chessprogramming.org/Perft_Results
 * @param depth
 * @param board
 * @return visited node number
 */
VIXEN_API BitBoard PerftTest(int depth, Board &board) noexcept;

VIXEN_API BitBoard Perft(int depth, Board &board) noexcept;
} // namespace Test
} // namespace Vixen
