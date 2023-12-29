#include "move_generator.hpp"
#include "defs.hpp"

#include <iostream>

namespace vixen
{
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
BitBoard PerftTest(int depth, Board &board) noexcept
{
    static BitBoard leafs = 0;
    if (depth == 0)
        return 1;

    BitBoard nodes = 0;
    const auto moves = board.GetMoveList<MoveTypes::ALL_MOVE>();

    for (const auto move : moves)
    {
        if (!board.MakeMove(move))
            continue;

        BitBoard cumNodes = leafs;
        nodes += Perft(depth - 1, board, leafs);
        board.TakeBack();
        BitBoard olds = leafs - cumNodes;
        std::cout << move;
        std::cout << ": " << olds << "\n";
    }
    return nodes;
}

BitBoard Perft(int depth, Board &board, BitBoard &leafs) noexcept
{
    if (depth == 0)
    {
        ++leafs;
        return 1;
    }

    BitBoard nodes = 0;
    const auto moves = board.GetMoveList<MoveTypes::ALL_MOVE>();

    for (const auto move : moves)
    {
        if (!board.MakeMove(move))
            continue;

        nodes += Perft(depth - 1, board, leafs);
        board.TakeBack();
    }
    return nodes;
}
} // namespace Test

template <Colors sideToMove> void MoveGenerator::GenerateQuietMoves(const Board &board) noexcept
{
    const auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_PAWN_INDEX)
                                                   : board.GetBitBoard(Constants::BLACK_PAWN_INDEX);
    const auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_KING_INDEX)
                                                   : board.GetBitBoard(Constants::BLACK_KING_INDEX);
    const auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_KNIGHT_INDEX)
                                                     : board.GetBitBoard(Constants::BLACK_KNIGHT_INDEX);
    const auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_BISHOP_INDEX)
                                                     : board.GetBitBoard(Constants::BLACK_BISHOP_INDEX);
    const auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_ROOK_INDEX)
                                                   : board.GetBitBoard(Constants::BLACK_ROOK_INDEX);
    const auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_QUEEN_INDEX)
                                                    : board.GetBitBoard(Constants::BLACK_QUEEN_INDEX);
    const auto targets = board.GetBitBoard(Constants::ALL_EMPTY_INDEX);
    const auto blockers = ~targets;

    constexpr auto pawnOffset = sideToMove == Colors::WHITE ? 8 : -8;
    constexpr auto promotionRanks =
        sideToMove == Colors::WHITE ? static_cast<BitBoard>(Ranks::RANK8) : static_cast<BitBoard>(Ranks::RANK1);
    constexpr auto doublePushStart =
        sideToMove == Colors::WHITE ? static_cast<BitBoard>(Ranks::RANK3) : static_cast<BitBoard>(Ranks::RANK6);
    const auto onePawnPush = PushPawns<sideToMove>(pawns) & ~blockers & ~promotionRanks;
    const auto doublePawnPush = PushPawns<sideToMove>((onePawnPush & doublePushStart)) & ~blockers;
    const auto pawnPromotion = PushPawns<sideToMove>(pawns) & ~blockers & promotionRanks;

    GeneratePawnMoves(pawnOffset, onePawnPush, static_cast<uint8_t>(MoveTypes::QUIET_MOVE));
    GeneratePawnMoves(2 * pawnOffset, doublePawnPush, static_cast<uint8_t>(MoveTypes::DOUBLE_PAWN_PUSH));
    GeneratePawnPromotionMoves(pawnOffset, pawnPromotion);

    GenerateAntiSliderMoves<static_cast<uint8_t>(MoveTypes::QUIET_MOVE)>(targets, knights,
                                                                         anti_slider_utils::knightAttack);
    GenerateAntiSliderMoves<static_cast<uint8_t>(MoveTypes::QUIET_MOVE)>(targets, kings, anti_slider_utils::kingAttack);

    GenerateSliderMoves<Slider::BISHOP, static_cast<uint8_t>(MoveTypes::QUIET_MOVE)>(bishops | queens, blockers,
                                                                                     targets);
    GenerateSliderMoves<Slider::ROOK, static_cast<uint8_t>(MoveTypes::QUIET_MOVE)>(rooks | queens, blockers, targets);

    GenerateCastlingMoves<sideToMove>(board);
}

template <Colors sideToMove> void MoveGenerator::GenerateCaptureMoves(const Board &board) noexcept
{
    const auto pawns = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_PAWN_INDEX)
                                                   : board.GetBitBoard(Constants::BLACK_PAWN_INDEX);
    const auto kings = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_KING_INDEX)
                                                   : board.GetBitBoard(Constants::BLACK_KING_INDEX);
    const auto knights = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_KNIGHT_INDEX)
                                                     : board.GetBitBoard(Constants::BLACK_KNIGHT_INDEX);
    const auto bishops = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_BISHOP_INDEX)
                                                     : board.GetBitBoard(Constants::BLACK_BISHOP_INDEX);
    const auto rooks = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_ROOK_INDEX)
                                                   : board.GetBitBoard(Constants::BLACK_ROOK_INDEX);
    const auto queens = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::WHITE_QUEEN_INDEX)
                                                    : board.GetBitBoard(Constants::BLACK_QUEEN_INDEX);
    const auto targets = sideToMove == Colors::WHITE ? board.GetBitBoard(Constants::ALL_BLACK_INDEX)
                                                     : board.GetBitBoard(Constants::ALL_WHITE_INDEX);
    const auto blockers = ~board.GetBitBoard(Constants::ALL_EMPTY_INDEX);

    constexpr auto pawnLeftCapture = sideToMove == Colors::WHITE ? 9 : -9;
    constexpr auto pawnRightCapture = sideToMove == Colors::WHITE ? 7 : -7;
    constexpr auto promotionRanks =
        sideToMove == Colors::WHITE ? static_cast<BitBoard>(Ranks::RANK8) : static_cast<BitBoard>(Ranks::RANK1);
    const auto pawnPromotionLeft = PawnCaptureLeft<sideToMove>(pawns) & targets & promotionRanks;
    const auto pawnPromotionRight = PawnCaptureRight<sideToMove>(pawns) & targets & promotionRanks;
    const auto enPassant = board.GetEnPassant() & (sideToMove == Colors::WHITE ? static_cast<BitBoard>(Ranks::RANK6)
                                                                               : static_cast<BitBoard>(Ranks::RANK3));

    GeneratePawnPromotionCaptureMoves(pawnLeftCapture, pawnPromotionLeft);
    GeneratePawnPromotionCaptureMoves(pawnRightCapture, pawnPromotionRight);
    GenerateAntiSliderMoves<static_cast<uint8_t>(MoveTypes::CAPTURE)>(
        targets & ~promotionRanks, pawns, anti_slider_utils::pawnAttack[static_cast<int>(sideToMove)]);
    GenerateAntiSliderMoves<static_cast<uint8_t>(MoveTypes::ENPASSANT)>(
        enPassant, pawns, anti_slider_utils::pawnAttack[static_cast<int>(sideToMove)]);

    GenerateAntiSliderMoves<static_cast<uint8_t>(MoveTypes::CAPTURE)>(targets, knights,
                                                                      anti_slider_utils::knightAttack);
    GenerateAntiSliderMoves<static_cast<uint8_t>(MoveTypes::CAPTURE)>(targets, kings, anti_slider_utils::kingAttack);
    GenerateSliderMoves<Slider::BISHOP, static_cast<uint8_t>(MoveTypes::CAPTURE)>(bishops | queens, blockers, targets);
    GenerateSliderMoves<Slider::ROOK, static_cast<uint8_t>(MoveTypes::CAPTURE)>(rooks | queens, blockers, targets);
}

template <Slider slider, uint8_t moveType>
void MoveGenerator::GenerateSliderMoves(BitBoard pieces, BitBoard blockers, BitBoard targets) noexcept
{
    auto attacks = Constants::EMPTY_BOARD;
    while (pieces)
    {
        const auto from = GetPosition(pieces);
        attacks |= slider == Slider::BISHOP ? slider_utils::GetBishopAttack(from, blockers)
                                            : slider_utils::GetRookAttack(from, blockers);
        attacks &= targets;
        while (attacks)
        {
            const auto to = GetPosition(attacks);
            moveList.emplace_back(from, to, moveType);
        }
    }
}

template <uint8_t moveType>
constexpr void MoveGenerator::GenerateAntiSliderMoves(
    BitBoard targets, BitBoard pieces, const std::array<BitBoard, Constants::SQUARE_NUMBER> &attackBoard) noexcept
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
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::QUEEN_PROMOTION));
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::ROOK_PROMOTION));
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::BISHOP_PROMOTION));
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::KNIGHT_PROMOTION));
    }
}

constexpr void MoveGenerator::GeneratePawnPromotionCaptureMoves(int offset, BitBoard promotion) noexcept
{
    while (promotion)
    {
        const unsigned to = GetPosition(promotion);
        const int from = static_cast<int>(to) - offset;
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::QUEEN_PROMO_CAPTURE));
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::ROOK_PROMO_CAPTURE));
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::BISHOP_PROMO_CAPTURE));
        moveList.emplace_back(static_cast<unsigned>(from), to, static_cast<uint8_t>(MoveTypes::KNIGHT_PROMO_CAPTURE));
    }
}

template <Colors sideToMove> constexpr void MoveGenerator::GenerateCastlingMoves(const Board &board) noexcept
{
    const auto currentCastlingRights = board.GetCastlingRights();
    const auto emptySquaresBitBoard = board.GetBitBoard(Constants::ALL_EMPTY_INDEX);

    const std::array castleEmptySquareLookUp = {IsBitSet(emptySquaresBitBoard, Squares::B8, Squares::C8, Squares::D8),
                                                IsBitSet(emptySquaresBitBoard, Squares::F8, Squares::G8),
                                                IsBitSet(emptySquaresBitBoard, Squares::B1, Squares::C1, Squares::D1),
                                                IsBitSet(emptySquaresBitBoard, Squares::F1, Squares::G1)};

    const auto isCastlingAvailable = [&board, currentCastlingRights,
                                      &castleEmptySquareLookUp](CastlingRights castlingRight) {
        const unsigned castlingRightBB = static_cast<uint8_t>(castlingRight);
        return (currentCastlingRights & castlingRightBB) &&
               castleEmptySquareLookUp[Constants::castlingLookUp[castlingRight]] &&
               !Check::IsInCheck<sideToMove>(board) &&
               !Check::IsSquareAttacked<sideToMove>(Constants::rookSquareAfterCastling[castlingRight], board);
    };

    if constexpr (sideToMove == Colors::WHITE)
    {
        if (isCastlingAvailable(CastlingRights::WKCA))
            moveList.emplace_back(static_cast<unsigned>(Squares::E1), static_cast<unsigned>(Squares::G1),
                                  static_cast<uint8_t>(MoveTypes::KING_CASTLE));

        if (isCastlingAvailable(CastlingRights::WQCA))
            moveList.emplace_back(static_cast<unsigned>(Squares::E1), static_cast<unsigned>(Squares::C1),
                                  static_cast<uint8_t>(MoveTypes::QUEEN_CASTLE));
    }
    else
    {
        if (isCastlingAvailable(CastlingRights::BKCA))
            moveList.emplace_back(static_cast<unsigned>(Squares::E8), static_cast<unsigned>(Squares::G8),
                                  static_cast<uint8_t>(MoveTypes::KING_CASTLE));

        if (isCastlingAvailable(CastlingRights::BQCA))
            moveList.emplace_back(static_cast<unsigned>(Squares::E8), static_cast<unsigned>(Squares::C8),
                                  static_cast<uint8_t>(MoveTypes::QUEEN_CASTLE));
    }
}

template <Colors sideToMove, MoveTypes moveType> void MoveGenerator::GenerateMoves(const vixen::Board &board) noexcept
{
    GenerateCaptureMoves<sideToMove>(board);
    if constexpr (moveType == MoveTypes::CAPTURE)
        return;

    GenerateQuietMoves<sideToMove>(board);
}

auto MoveGenerator::GetLegalMoveList(Board &board) const noexcept
{
    FixedList<Move> allLegalMoves;
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

template void MoveGenerator::GenerateMoves<Colors::WHITE, MoveTypes::CAPTURE>(const Board &board) noexcept;

template void MoveGenerator::GenerateMoves<Colors::BLACK, MoveTypes::CAPTURE>(const Board &board) noexcept;

template void MoveGenerator::GenerateMoves<Colors::WHITE, MoveTypes::ALL_MOVE>(const Board &board) noexcept;

template void MoveGenerator::GenerateMoves<Colors::BLACK, MoveTypes::ALL_MOVE>(const Board &board) noexcept;
} // namespace vixen
