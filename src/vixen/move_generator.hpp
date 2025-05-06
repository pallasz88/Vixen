#ifndef SRC_VIXEN_MOVE_GENERATOR_HPP_
#define SRC_VIXEN_MOVE_GENERATOR_HPP_

#include <cassert>

#include "defs.hpp"
#include "move.hpp"
#include "fixed_list.hpp"

namespace vixen
{

class Board;
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
    
    /**
     * @brief Clears the move list.
     * 
     */
    constexpr void clear() noexcept
    {
        moveList.clear();
    }

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

#endif // SRC_VIXEN_MOVE_GENERATOR_HPP_