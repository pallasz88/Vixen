#include "move_generator.h"

#include <iostream>

namespace Vixen
{
namespace Test
{
BitBoard leafs = 0;

/**
 * Helper function to validate move generation:
 *  - Perft method: https://www.chessprogramming.org/Perft
 *  - Perft results: https://www.chessprogramming.org/Perft_Results
 * @param depth
 * @param board
 * @return visited node number
 */
BitBoard PerftTest(int depth, Board &board) noexcept
{
    if (depth == 0)
        return 1;

    BitBoard nodes = 0;
    const auto generator = board.CreateGenerator<ALL_MOVE>();

    for (const auto move : generator.GetMoveList())
    {
        if (!board.MakeMove(move))
            continue;

        BitBoard cumNodes = leafs;
        nodes += Perft(depth - 1, board);
        board.TakeBack();
        BitBoard olds = leafs - cumNodes;
        std::cout << move;
        std::cout << ": " << olds << "\n";
    }
    return nodes;
}

BitBoard Perft(int depth, Board &board) noexcept
{
    if (depth == 0)
    {
        ++leafs;
        return 1;
    }

    BitBoard nodes = 0;
    const auto generator = board.CreateGenerator<ALL_MOVE>();

    for (const auto move : generator.GetMoveList())
    {
        if (!board.MakeMove(move))
            continue;

        nodes += Perft(depth - 1, board);
        board.TakeBack();
    }
    return nodes;
}
} // namespace Test

template <Colors sideToMove> void MoveGenerator::GenerateQuietMoves(const Board &board) noexcept
{
    const auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard('P') : board.GetBitBoard('p');
    const auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard('K') : board.GetBitBoard('k');
    const auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard('N') : board.GetBitBoard('n');
    const auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard('B') : board.GetBitBoard('b');
    const auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard('R') : board.GetBitBoard('r');
    const auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard('Q') : board.GetBitBoard('q');
    const auto targets = board.GetBitBoard(' ');
    const auto blockers = ~targets;

    constexpr auto pawnOffset = sideToMove == Colors::WHITE ? 8 : -8;
    constexpr auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
    constexpr auto doublePushStart = sideToMove == Colors::WHITE ? RANK3 : RANK6;
    const auto onePawnPush = PushPawns<sideToMove>(pawns) & ~blockers & ~promotionRanks;
    const auto doublePawnPush = PushPawns<sideToMove>((onePawnPush & doublePushStart)) & ~blockers;
    const auto pawnPromotion = PushPawns<sideToMove>(pawns) & ~blockers & promotionRanks;

    GeneratePawnMoves(pawnOffset, onePawnPush, QUIET_MOVE);
    GeneratePawnMoves(2 * pawnOffset, doublePawnPush, DOUBLE_PAWN_PUSH);
    GeneratePawnPromotionMoves(pawnOffset, pawnPromotion);

    GenerateAntiSliderMoves<QUIET_MOVE>(targets, knights, AntSliderUtils::knightAttack);
    GenerateAntiSliderMoves<QUIET_MOVE>(targets, kings, AntSliderUtils::kingAttack);

    GenerateSliderMoves<Slider::BISHOP, QUIET_MOVE>(bishops | queens, blockers, targets);
    GenerateSliderMoves<Slider::ROOK, QUIET_MOVE>(rooks | queens, blockers, targets);

    GenerateCastlingMoves<sideToMove>(board);
}

template <Colors sideToMove> void MoveGenerator::GenerateCaptureMoves(const Board &board) noexcept
{
    const auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard('P') : board.GetBitBoard('p');
    const auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard('K') : board.GetBitBoard('k');
    const auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard('N') : board.GetBitBoard('n');
    const auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard('B') : board.GetBitBoard('b');
    const auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard('R') : board.GetBitBoard('r');
    const auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard('Q') : board.GetBitBoard('q');
    const auto targets = sideToMove == Colors::WHITE ? board.GetBitBoard('S') : board.GetBitBoard('F');
    const auto blockers = ~board.GetBitBoard(' ');

    constexpr auto pawnLeftCapture = sideToMove == Colors::WHITE ? 9 : -9;
    constexpr auto pawnRightCapture = sideToMove == Colors::WHITE ? 7 : -7;
    constexpr auto promotionRanks = sideToMove == Colors::WHITE ? RANK8 : RANK1;
    const auto pawnPromotionLeft = PawnCaptureLeft<sideToMove>(pawns) & targets & promotionRanks;
    const auto pawnPromotionRight = PawnCaptureRight<sideToMove>(pawns) & targets & promotionRanks;
    const auto enPassant = board.GetEnPassant() & (sideToMove == Colors::WHITE ? RANK6 : RANK3);

    GeneratePawnPromotionCaptureMoves(pawnLeftCapture, pawnPromotionLeft);
    GeneratePawnPromotionCaptureMoves(pawnRightCapture, pawnPromotionRight);
    GenerateAntiSliderMoves<CAPTURE>(targets & ~promotionRanks, pawns,
                                     AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)]);
    GenerateAntiSliderMoves<ENPASSANT>(enPassant, pawns, AntSliderUtils::pawnAttack[static_cast<int>(sideToMove)]);

    GenerateAntiSliderMoves<CAPTURE>(targets, knights, AntSliderUtils::knightAttack);
    GenerateAntiSliderMoves<CAPTURE>(targets, kings, AntSliderUtils::kingAttack);
    GenerateSliderMoves<Slider::BISHOP, CAPTURE>(bishops | queens, blockers, targets);
    GenerateSliderMoves<Slider::ROOK, CAPTURE>(rooks | queens, blockers, targets);
}

template <Slider slider, uint8_t moveType>
void MoveGenerator::GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets) noexcept
{
    auto attacks = Constants::EMPTY_BOARD;
    while (pieces)
    {
        const auto from = GetPosition(pieces);
        attacks |= slider == Slider::BISHOP ? SliderUtils::GetBishopAttack(from, blockers)
                                            : SliderUtils::GetRookAttack(from, blockers);
        attacks &= targets;
        while (attacks)
        {
            const auto to = GetPosition(attacks);
            moveList.emplace_back(from, to, moveType);
        }
    }
}

template <uint8_t moveType>
constexpr void MoveGenerator::GenerateAntiSliderMoves(BitBoard targets, BitBoard pieces, const BitBoard *attackBoard) noexcept
{
    auto attacks = Constants::EMPTY_BOARD;
    while (pieces)
    {
        const unsigned from = GetPosition(pieces);
        attacks |= attackBoard[from];
        attacks &= targets;
        while (attacks)
        {
            const unsigned to = GetPosition(attacks);
            moveList.emplace_back(from, to, moveType);
        }
    }
}

constexpr void MoveGenerator::GeneratePawnMoves(int pawnOffset, BitBoard pawnPushed, uint8_t moveType) noexcept
{
    while (pawnPushed)
    {
        const unsigned to = GetPosition(pawnPushed);
        const int from = static_cast<int>(to) - pawnOffset;
        moveList.emplace_back(static_cast<unsigned>(from), to, moveType);
    }
}

constexpr void MoveGenerator::GeneratePawnPromotionMoves(int offset, BitBoard promotion) noexcept
{
    while (promotion)
    {
        const unsigned to = GetPosition(promotion);
        const int from = static_cast<int>(to) - offset;
        moveList.emplace_back(static_cast<unsigned>(from), to, QUEEN_PROMOTION);
        moveList.emplace_back(static_cast<unsigned>(from), to, ROOK_PROMOTION);
        moveList.emplace_back(static_cast<unsigned>(from), to, BISHOP_PROMOTION);
        moveList.emplace_back(static_cast<unsigned>(from), to, KNIGHT_PROMOTION);
    }
}

constexpr void MoveGenerator::GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion) noexcept
{
    while (promotion)
    {
        const unsigned to = GetPosition(promotion);
        const int from = static_cast<int>(to) - offset;
        moveList.emplace_back(static_cast<unsigned>(from), to, QUEEN_PROMO_CAPTURE);
        moveList.emplace_back(static_cast<unsigned>(from), to, ROOK_PROMO_CAPTURE);
        moveList.emplace_back(static_cast<unsigned>(from), to, BISHOP_PROMO_CAPTURE);
        moveList.emplace_back(static_cast<unsigned>(from), to, KNIGHT_PROMO_CAPTURE);
    }
}

template <Colors sideToMove> constexpr void MoveGenerator::GenerateCastlingMoves(const Board &board) noexcept
{
    const auto currentCastlingRights = board.GetCastlingRights();
    const auto emptySquaresBitBoard = board.GetBitBoard(' ');

    const std::array castleEmptySquareLookUp = {
        IsBitSet(emptySquaresBitBoard, B8, C8, D8), IsBitSet(emptySquaresBitBoard, F8, G8),
        IsBitSet(emptySquaresBitBoard, B1, C1, D1), IsBitSet(emptySquaresBitBoard, F1, G1)};

    const auto isCastlingAvailable = [&board, currentCastlingRights,
                                      &castleEmptySquareLookUp](CastlingRights castlingRight) {
        const unsigned castlingRightBB = static_cast<uint8_t>(castlingRight);
        return (currentCastlingRights & castlingRightBB) &&
               castleEmptySquareLookUp[Constants::castlingLookUp[castlingRight]] &&
               !Check::IsInCheck<sideToMove>(board) &&
               !Check::IsSquareAttacked<sideToMove>(Constants::rookSquareAfterCastling[castlingRight], board);
    };

    if (sideToMove == Colors::WHITE)
    {
        if (isCastlingAvailable(CastlingRights::WKCA))
            moveList.emplace_back(E1, G1, KING_CASTLE);

        if (isCastlingAvailable(CastlingRights::WQCA))
            moveList.emplace_back(E1, C1, QUEEN_CASTLE);
    }
    else
    {
        if (isCastlingAvailable(CastlingRights::BKCA))
            moveList.emplace_back(E8, G8, KING_CASTLE);

        if (isCastlingAvailable(CastlingRights::BQCA))
            moveList.emplace_back(E8, C8, QUEEN_CASTLE);
    }
}

template <Colors sideToMove, uint8_t moveType> void MoveGenerator::GenerateMoves(const Vixen::Board &board) noexcept
{
    GenerateCaptureMoves<sideToMove>(board);
    if (moveType == CAPTURE)
        return;

    GenerateQuietMoves<sideToMove>(board);
}

std::vector<Move> MoveGenerator::GetLegalMoveList(Board &board) const noexcept
{
    std::vector<Move> allLegalMoves;
    const auto addLegal = [&board, &allLegalMoves](Move move) {
        if (board.MakeMove(move))
        {
            allLegalMoves.emplace_back(move);
            board.TakeBack();
        }
    };

    std::for_each(begin(moveList), end(moveList), addLegal);
    return allLegalMoves;
}

template void MoveGenerator::GenerateCaptureMoves<Colors::WHITE>(const Board &) noexcept;

template void MoveGenerator::GenerateCaptureMoves<Colors::BLACK>(const Board &) noexcept;

template void MoveGenerator::GenerateQuietMoves<Colors::WHITE>(const Board &) noexcept;

template void MoveGenerator::GenerateQuietMoves<Colors::BLACK>(const Board &) noexcept;

template void MoveGenerator::GenerateMoves<Colors::WHITE, CAPTURE>(const Board &board) noexcept;

template void MoveGenerator::GenerateMoves<Colors::BLACK, CAPTURE>(const Board &board) noexcept;

template void MoveGenerator::GenerateMoves<Colors::WHITE, ALL_MOVE>(const Board &board) noexcept;

template void MoveGenerator::GenerateMoves<Colors::BLACK, ALL_MOVE>(const Board &board) noexcept;
} // namespace Vixen
